# ReorderLayoutItems
Reorder Qt layout items in the .ui file. This is a workaround dealing with the issues concerning the tab-order linkedlist not behaving as expected.

The project uses Qt to handle the DOM files. Easiest to build it is to use qmake on the *.pro file.
It has basic argument handling, which allows for verbose printing, and saving the previous file to a backup file in the same folder as the original file.

The output of the reordered UI file goes to the standard output.
