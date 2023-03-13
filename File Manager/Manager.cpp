#include "Manager.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <windows.h>
using std::cin;
using std::cout;
using std::endl;


//Dir::Dir() : Dir("new") {}
Dir::Dir(string dir_name) : name{ dir_name }, size{ 0 }, back{ nullptr } { }
Dir::~Dir() {
	for (auto& i : dirs) {
		delete i;
	}
	for (auto& i : files) {
		delete i;
	}
}

void Dir::load(std::ifstream& info) {

	unsigned int amount;
	

	string temp_name;
	unsigned int temp_size = 0;
	File* temp_file = nullptr;
	Dir* temp_dir = nullptr;

	info >> amount;
	for (size_t i = 0; i < amount; i++) {
		if (info.eof()) return;
		info >> temp_name >> temp_size;
		temp_file = new File(temp_name);
		temp_file->set_size(temp_size);
		temp_file->set_back(this);
		files.push_back(temp_file);
		temp_file = nullptr;
	}

	info >> amount;
	for (size_t i = 0; i < amount; i++) {
		if (info.eof()) return;
		info >> temp_name >> temp_size;
		temp_dir = new Dir(temp_name);
		temp_dir->size = temp_size;
		temp_dir->back = this;
		dirs.push_back(temp_dir);
		temp_dir->load(info);
		temp_dir = nullptr;
	}
}

void Dir::command(Dir** current, string cmd, Dir** copied_dir, bool& cut_dir, File** copied_file, bool& cut_file, Dir** disk) {

	Dir* temp_dir;

	if (cmd == "help") (* current)->help();
	if (cmd == "cls") system("cls");
	if (cmd == "show") (*current)->show();
	if (cmd == "back") {
		temp_dir = (*current)->back;
		if (temp_dir) *current = temp_dir;
	}
	if (cmd == "tree") (*current)->tree();
	if (cmd == "paste_dir") (*current)->paste_dir(copied_dir, cut_dir);
	if (cmd == "paste_file") (*current)->paste_file(copied_file, cut_file);
	if (cmd == "md") (*current)->make_dir();
	if (cmd == "make_file") (*current)->make_file();
	if (cmd == "cd") {
		temp_dir = (*current)->cd();
		if (temp_dir) (*current) = temp_dir;
	}
	if (cmd == "rd") (*current)->remove_dir();
	if (cmd == "del") (*current)->remove_file();
	if (cmd == "resize") (*current)->resize_file();
	if (cmd == "rnm_dir") (*current)->rename_dir();
	if (cmd == "rnm_file") (*current)->rename_file();
	if (cmd == "copy_dir") {
		*copied_dir = (*current)->copy_dir();
		cut_dir = false;
	}
	if (cmd == "cut_dir") {
		*copied_dir = (*current)->cut_dir();
		cut_dir = true;
	}
	if (cmd == "copy_file") {
		*copied_file = (*current)->copy_file();
		cut_file = false;
	}
	if (cmd == "cut_file") {
		*copied_file = (*current)->cut_file();
		cut_file = true;
	}
	if (cmd == "find") {
		temp_dir = (*current)->find_path();
		if (temp_dir) (*current) = temp_dir;
	}
	if (cmd == "save") {
		std::ofstream info;
		info.open("data", std::ios::out);
		(* disk)->save(info);
		info.close();
	}
	if (cmd == "color") {
		(*current)->color();
	}
	if (cmd == "colors") {
		(*current)->colors();
	}
	if (cmd == "exit") {
		delete (*disk);
		(* disk) = nullptr;
	}
}

void Dir::help() {
	cout << "md\t\t" << "create directory\n";
	cout << "make_file\t" << "create file\n";
	cout << endl;
	cout << "cd\t\t" << "changes current directory\n";
	cout << "back\t\t" << "go to the previous directory\n";
	cout << endl;
	cout << "rnm_dir\t\t" << "rename directory\n";
	cout << "rnm_file\t" << "rename file\n";
	cout << endl;
	cout << "show\t\t" << "shows a list of files and subdirectories in current directory\n";
	cout << "tree\t\t" << "shows the folder structure of a current path\n";
	cout << endl;
	cout << "rd\t\t" << "removes a directory\n";
	cout << "del\t\t" << "removes a file\n";
	cout << endl;
	cout << "resize\t\t" << "set size of file\n";
	cout << endl;
	cout << "copy_dir\t" << "copies a directory\n";
	cout << "cut_dir\t\t" << "cuts a directory\n";
	cout << endl;
	cout << "copy_file\t" << "copies a file\n";
	cout << "cut_file\t" << "cuts a file\n";
	cout << "paste_dir\t" << "paste a copied/cut directory into the current directory\n";
	cout << "paste_file\t" << "paste a copied/cut file into the current directory\n";
	cout << endl;
	cout << "find\t\t" << "finds directory by path\n";
	cout << endl;
	cout << "cls\t\t" << "clears console screen\n";
	cout << endl;
	cout << "color\t\t" << "changes the color of text\n";
	cout << "colors\n" << "shows all color id's\n";
	cout << endl;
	cout << "exit\t\t" << "exits the application\n";
}
Dir* Dir::recurs_find(const vector<string>& dir_names, int count) {
	if (dir_names.size() == count) return this;
	Dir* temp = search_dir(dir_names[count]);
	if (temp) return temp->recurs_find(dir_names, count + 1);
	else return nullptr;
}
Dir* Dir::find_path() {
	vector<string> dir_names;
	string temp;
	int choice=0;
	do {
		std::copy(dir_names.begin(), dir_names.end(), std::ostream_iterator<string>(cout, "\\"));
		cout << endl;
		cout << "Add a dir to path - (1)\n";
		cout << "Remove last - (2)\n";
		cout << "Continue - (0)\n> ";
		cin >> choice;
		if (choice == 1){
			cout << "Enter dir name: ";
			cin >> temp;
			dir_names.push_back(temp);
		}
		if (choice == 2) {
			if (!dir_names.empty()) dir_names.pop_back();
		}
	} while (choice != 0);

	return recurs_find(dir_names, 0);

}
string Dir::current_path() {
	string path = this->name;
	Dir* temp = this;
	while (temp->back) {
		temp = temp->back;
		path = temp->name + "\\" + path;
	}
	return path;
}

void Dir::remove_dir() {
	Dir* dir = search_dir();
	if (dir) {
		auto res = find(this->dirs.begin(), this->dirs.end(), dir);
		if (res != this->dirs.end()) {
			this->dirs.erase(res);
		}
		delete dir;
	}
}
void Dir::remove_file () {
	File* dir = search_file();
	if (dir) {
		auto res = find(this->files.begin(), this->files.end(), dir);
		if (res != this->files.end()) {
			this->files.erase(res);
		}
		delete dir;
	}
}
Dir* Dir::search_dir() {
	string dir_name;
	cout << "Enter name of dir: ";
	cin >> dir_name;
	return search_dir(dir_name);
}
Dir* Dir::search_dir(string dir_name) {
	auto res = find_if(dirs.begin(), dirs.end(), [dir_name](const Dir* dir) {return (dir->name == dir_name); });
	if (res == dirs.end()) {
		return nullptr;
	}
	return *res;
}
File* Dir::search_file() {
	string file_name;
	cout << "Enter name of file: ";
	cin >> file_name;
	return search_file(file_name);
}
File* Dir::search_file(string file_name) {
	auto res = find_if(files.begin(), files.end(), [file_name](const File* file) {return (file->get_name() == file_name); });
	if (res == files.end()) {
		return nullptr;
	}
	return *res;
}
void Dir::make_dir() {
	string dir_name;
	cout << "Enter a name of dir: ";
	cin >> dir_name;
	if (!search_dir(dir_name)) {
		Dir* new_dir = new Dir(dir_name);
		new_dir->size = 0;
		new_dir->set_back(this);
		dirs.push_back(new_dir);
		new_dir = nullptr;
		cout << "Created dir\n";
	}
	else {
		cout << "Dir \"" << dir_name << "\" already exist\n";
	}
}
void Dir::make_file() {
	string file_name;
	cout << "Enter a name of file: ";
	cin >> file_name;
	if (!search_file(file_name)) {
		File* new_file = new File(file_name);
		new_file->set_back(this);
		files.push_back(new_file);
		new_file = nullptr;
		cout << "Created file\n";
	}
	else {
		cout << "File \"" << file_name << "\" already exist\n";
	}
}
void Dir::calc_size() {
	int res = 0;
	for (auto& i : files) {
		res += i->get_size();
	}
	for (auto& i : dirs) {
		i->calc_size();
		res += i->size;
	}
	size = res;
}
void Dir::show() {
	cout << "TYPE\tSIZE\tNAME\n";
	for (auto& i : dirs) {
		i->calc_size();
		cout << "<dir>\t" << i->size << "\t" << i->name << endl;
	}
	for (auto& i : files) {
		cout << "<file>\t" << i->get_size() << "\t" << i->get_name() << endl;
	}
}
void Dir::tree(int level) {
	for (int i = 0; i < level-1; i++) cout << "|   ";
	if (level) cout << "|---";
	cout << name << endl;
	for (auto& i : dirs) {
		i->tree(level+1);
	}
}
Dir* Dir::cd() {
	return this->search_dir();
}
void Dir::set_back(Dir* back) {
	this->back = back;
}
Dir* Dir::get_back() const {
	return back;
}
void Dir::rename_dir() {
	Dir* dir = search_dir();
	if (dir) {
		string new_name;
		cout << "Enter a new name: ";
		cin >> new_name;
		if (search_dir(new_name)) {
			cout << "Dir with such name already exist\n";
			return;
		}
		dir->name = new_name;
		cout << "Renamed\n";
	}
}
void Dir::rename_file(){
	File* file = search_file();
	if (file) {
		string new_name;
		cout << "Enter a new name: ";
		cin >> new_name;
		if (search_file(new_name)) {
			cout << "File with such name already exist\n";
			return;
		}
		file->set_name(new_name);
		cout << "Renamed\n";
	}
}
void Dir::resize_file() {
	File* file = search_file();
	if (file) {
		unsigned int new_size;
		cout << "Enter a new size: ";
		cin >> new_size;
		file->set_size(new_size);
	}
}
string Dir::get_name() { return name; }

void Dir::recurs_copy(Dir& dir) {
	Dir* temp_dir;
	File* temp_file;
	for (auto i : dir.dirs) {
		temp_dir = new Dir(i->name);
		this->dirs.push_back(temp_dir);
		temp_dir->recurs_copy(*i);
		temp_dir->back = this;
	}
	for (auto i : dir.files) {
		temp_file = new File(i->get_name());
		temp_file->set_back(this);
		temp_file->set_size(i->get_size());
		this->files.push_back(temp_file);
	}
	temp_dir = nullptr;
	temp_file = nullptr;
}

Dir* Dir::copy_dir(){
	Dir* res;
	Dir* temp = search_dir();
	if (temp) {
		res = new Dir(temp->name);
		res->recurs_copy(*temp);
		return res;
		cout << "Copied\n";
	}
	return nullptr;
}
Dir* Dir::cut_dir() {
	Dir* res = search_dir();
	if (res) {
		cout << "Cut\n";
		return res;
	}
	return nullptr;
}
File* Dir::copy_file() {
	File* res;
	File* temp = search_file();
	if (temp) {
		res = new File(temp->get_name());
		res->set_size(temp->get_size());
		res->set_back(this);
		cout << "Copied\n";
		return res;
	}
	return nullptr;
}
File* Dir::cut_file() {
	File* res = search_file();
	if (res) {
		cout << "Cut\n";
		return res;
	}
	return nullptr;
}
void Dir::paste_dir(Dir** copied_dir, bool& cut_dir) {
	if (*copied_dir) {
		if (search_dir((*copied_dir)->name)) {
			cout << "Dir with such name already exist\n";
			return;
		}
		if (cut_dir) {
			Dir* temp = (*copied_dir)->back;
			auto res = find_if(temp->dirs.begin(), temp->dirs.end(), [&copied_dir](const Dir* dir) {
				return (dir->name == (*copied_dir)->name);
				});
			if (res != temp->dirs.end()) {
				temp->dirs.erase(res);
			}
		}
		(*copied_dir)->back = this;
		this->dirs.push_back(*copied_dir);
		(* copied_dir) = nullptr;
		cut_dir = false;
		cout << "Pasted\n";
	}
	else {
		cout << "No dir was copied\n";
	}
}
void Dir::paste_file(File** copied_file, bool& cut_file) {
	if (*copied_file) {
		if (search_file((*copied_file)->get_name())) {
			cout << "File with such name already exist\n";
			return;
		}
		if (cut_file) {
			Dir* temp = (*copied_file)->get_back();
			auto res = find_if(temp->files.begin(), temp->files.end(), [&copied_file](const File* file) {
				return (file->get_name() == (*copied_file)->get_name());
				});
			if (res != temp->files.end()) {
				temp->files.erase(res);
			}
		}
		(*copied_file)->set_back(this);
		this->files.push_back(*copied_file);
		(* copied_file) = nullptr;
		cut_file = false;
		cout << "Pasted\n";
	}
	else {
		cout << "No file was copied\n";
	}
}

void Dir::save(std::ofstream& info) {
	info << this->files.size() << endl;
	for (auto& file : this->files) {
		info << file->get_name() << "\t" << file->get_size() << endl;
	}
	info << this->dirs.size() << endl;
	for (auto& dir : dirs) {
		info << dir->name << "\t" << dir->size << endl;
		dir->save(info);
	}
}

void Dir::color() {
	int id;
	cout << "Enter color id: ";
	cin >> id;
	cin.clear();
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, id);
}

void Dir::colors() {
	cout <<
		"black      0\n" <<
		"dark_blue  1\n" <<
		"dark_green 2\n" <<
		"light_blue 3\n" <<
		"dark_red   4\n" <<
		"magenta    5\n" <<
		"orange     6\n" <<
		"light_gray 7\n" <<
		"gray       8\n" <<
		"blue       9\n" <<
		"green     10\n" <<
		"cyan      11\n" <<
		"red       12\n" <<
		"pink      13\n" <<
		"yellow    14\n" <<
		"white     15\n";
}



//File::File() : File("new") {}
File::File(string file_name) : name{ file_name }, size{ 1 }, back{nullptr} {}
//File::~File() { delete back; }
Dir* File::get_back() const {
	return back;
}
void File::set_back(Dir* back) {
	this->back = back;
}
string File::get_name() const {
	return name;
}
void File::set_name(string new_name) {
	name = new_name;
}
unsigned int File::get_size() const {
	return size;
}
void File::set_size(unsigned int new_size) {
	size = new_size;
}
