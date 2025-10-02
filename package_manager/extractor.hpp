#ifndef EXTRACTOR_HPP
#define EXTRACTOR_HPP

#include <string>

/** \brief Extracts entire file containment to the directory.
 *  \param file Path to the file need to be extracted.
 *  \param out_directory Path to the existing directory to extract.
 *  \return Result of extraction.
 *  \retval true The data successfully extracted.
 *  \retval false Something went wrong during extraction. */
bool extract(std::string file, std::string out_directory);

#endif // EXTRACTOR_HPP
