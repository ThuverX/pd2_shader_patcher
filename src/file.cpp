#include "file.h"

std::vector<unsigned char> file_read(std::string path)
{
	std::ifstream file(path, std::ios::binary);

	if (file.is_open())
	{
		return std::vector<unsigned char>(std::istreambuf_iterator<char>(file), {});
	}

    std::string msg = "Failed to open the file: " + path;

    PD2HOOK_LOG_ERROR(msg.c_str());

	return std::vector<unsigned char>();
}

void file_write(std::string path, std::vector<unsigned char> data)
{
	std::ofstream outfile(path, std::ofstream::binary);

	outfile.write((char*)data.data(), data.size());

	outfile.close();
}

std::vector<std::string> get_mod_folders()
{
	std::vector<std::string> mod_folders;

	std::string mods_path = "mods";

	for (const auto& entry : std::filesystem::directory_iterator(mods_path))
	{
		if (entry.is_directory())
		{
			mod_folders.push_back(entry.path().string());
		}
	}

	return mod_folders;
}

std::vector<std::string> find_files(std::string path, std::string name)
{
	std::vector<std::string> files;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_regular_file())
		{
			std::string filename = entry.path().filename().string();

			if (filename == name)
			{
				files.push_back(entry.path().string());
			}
		}
	}

	return files;
}