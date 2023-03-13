#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;

class Dir;
class File;

class Dir{
	private:
		string name;
		unsigned int size;
		vector<Dir*> dirs;
		vector<File*> files;
		Dir* back;
	public:
		//Dir();
		Dir(string name);
		~Dir();
		void load(std::ifstream& info);
		Dir* recurs_find(const vector<string>& dir_names, int count);
		Dir* find_path();
		string current_path();
		void command(Dir** current, string cmd, Dir** copied_dir, bool& cut_dir, File** copied_file, bool& cut_file, Dir** disk);
		void help();
		Dir* search_dir();
		Dir* search_dir(string dir_name);
		File* search_file();
		File* search_file(string file_name);
		void remove_dir();
		void remove_file();
		void make_dir();
		void make_file();
		void set_back(Dir* go_back);
		Dir* get_back() const;
		void rename_dir();
		void rename_file();
		void resize_file();
		string get_name();
		void recurs_copy(Dir& dir);
		Dir* cut_dir();
		Dir* copy_dir();
		File* cut_file ();
		File* copy_file ();
		void paste_dir(Dir** copied_dir, bool& cut_dir);
		void paste_file(File** copied_file, bool& cut_file);
		void calc_size();
		void show();
		void tree(int level=0);
		Dir* cd();
		void save(std::ofstream& info);
		void color();
		void colors();
};

class File {
private:
	Dir* back;
	string name;
	unsigned int size;
public:
	//File();
	File(string file_name);
	//~File();
	void set_name(string new_name);
	string get_name() const;
	void set_back(Dir* back);
	Dir* get_back() const;
	unsigned int get_size() const;
	//void rename(string new_name);
	void set_size(unsigned int new_size);
};