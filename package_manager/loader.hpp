#ifndef LOADER_HPP
#define LOADER_HPP

#include <string>

/** \brief Starts downloading the file from the internet.
 *  \details Checks the access to the directory and create if needed.
 *           Then creates a worker thread in the download pool and start
 *           downloading here.
 *  \param url URL of file to download.
 *  \param dir Directory to save the file.
 *  \return Status of download request.
 *  \retval true The file started to download.
 *  \retval false Unable to download or save the file. */
bool download_start(std::string url, std::string dir);

/** \brief Checks the progress of the file download.
 *  \details Checks out if the download pool have active threads that
 *           downloads this file and return current state.
 *  \param url URL that should be inside inner download pool.
 *  \return State of downloading 0..1
 *  \retval 0 Download did not started or there's no active downloads for
 *            this URL.
 *  \retval 1 Download of this file finished.
 *  \note 1 may be returned only once, then this URL record clears and
 *        further requests for this URL should return 0 */
float download_progress(std::string url);

#endif // LOADER_HPP
