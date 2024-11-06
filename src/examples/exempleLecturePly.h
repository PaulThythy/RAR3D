#ifndef exemple_lecture_ply_H
#define exemple_lecture_ply_H

#include <string>

void write_ply_example(const std::string &filename);
void read_ply_file(const std::string &filepath, bool preload_into_memory = true);

#endif