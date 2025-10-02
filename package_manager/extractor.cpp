#include "extractor.hpp"

#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <iostream>

static bool copy_data(struct archive* rd, struct archive* wr)
{
	while (true)
	{
		const void* buff = nullptr;
		size_t size = 0;
		la_int64_t offset = 0;

		int res = ARCHIVE_OK;
		if ((res = archive_read_data_block(rd, &buff, &size, &offset))
				== ARCHIVE_EOF)
		{
			return true;
		}
		else if (res != ARCHIVE_OK)
		{
			return false;
		}

		if (archive_write_data_block(wr, buff, size, offset)
				!= ARCHIVE_OK)
		{
			return false;
		}
	}

	return true;
}

static void cleanup(struct archive* rd, struct archive* wr)
{
	archive_read_close(rd);
	archive_read_free(rd);
	archive_write_close(wr);
	archive_write_free(wr);
}

bool extract(std::string file, std::string out_directory)
{
	std::filesystem::create_directories(out_directory);

	struct archive* rd = archive_read_new();
	struct archive* wr = archive_write_disk_new();

	archive_read_support_filter_all(rd);
	archive_read_support_format_all(rd);

	archive_write_disk_set_options(
		wr,
		ARCHIVE_EXTRACT_TIME |
		ARCHIVE_EXTRACT_PERM |
		ARCHIVE_EXTRACT_ACL |
		ARCHIVE_EXTRACT_FFLAGS);

	int res = ARCHIVE_OK;
	if ((res = archive_read_open_filename(rd, file.c_str(), 10240))
			!= ARCHIVE_OK)
	{
		archive_read_free(rd);
		archive_write_free(wr);
		return false;
	}

	struct archive_entry* item;
	while ((res = archive_read_next_header(rd, &item)) == ARCHIVE_OK)
	{
		std::string path = out_directory;
		path.append("/").append(archive_entry_pathname(item));
		std::filesystem::create_directories(
				std::filesystem::path(path).parent_path()
			);
		archive_entry_set_pathname(item, path.c_str());

		if ((res = archive_write_header(wr, item)) != ARCHIVE_OK)
		{
			cleanup(rd, wr);
			return false;
		}

		if ((res = archive_entry_size(item)) > 0 && !copy_data(rd, wr))
		{
			cleanup(rd, wr);
			return false;
		}

		if ((res = archive_write_finish_entry(wr)) != ARCHIVE_OK)
		{
			cleanup(rd, wr);
			return false;
		}
	}

	if (res != ARCHIVE_EOF)
	{
		cleanup(rd, wr);
		return false;
	}

	cleanup(rd, wr);
	return true;
}
