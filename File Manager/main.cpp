#include "Manager.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

int main() {
	Dir* disk = new Dir("A");
	Dir* folder = disk;

	Dir* temp_dir = nullptr;
	File* temp_file = nullptr;

	Dir* copied_dir = nullptr;
	File* copied_file = nullptr;
	bool cut_dir = false;
	bool cut_file = false;
	string cmd;

	cout << "Type \"help\" for more info\n\n";

	while (true) {
		cout << folder->current_path() << "> ";
		cin >> cmd;
		folder->command(&folder, cmd, &copied_dir, cut_dir, &copied_file, cut_file);
	}
	if (disk) delete disk;
}