IntroductionAddress Book:It's a command line application developed by C programming language. You can manage address book by adding, catting and removing address entries."ls"can check the people's name."!help"can know how to use it;"quit"is to quit the software. 

Development environment:

Linux version 2.6.32-17-generic

gcc version 4.4.3 (Ubuntu 4.4.3-4ubuntu5)

start interactive command line application:

sh> ./ab

ab> _

use ls command to list the items in current position

ab> ls

ab> entries

use cd command to go to the entry like go to a directory

ab> cd entries

ab> ls

ab> lilei hanmeimei

use cat command to display the item data

ab> cat lilei

ab> "lilei" : { "age": 27, "mobile" : "13700000000", "address" : "Earth somewhere" }

use add command to add new address entry to JSON

ab> cd entries

ab> add

key: xiaoming

value: { "age": 28, "mobile" : "13700000002", "address" : "Earth somewhere too" }

address entry added

ab> ls

ab> lilei hanmeimei xiaoming

Note: the key should be identical, if trying to add duplicated key, just display an error message

use remove command to get one or more address entries

ab> cd entries

ab> remove

please give the key: xiaoming

xiaoming was deleted from JSON

Note: if trying to delete an non-exist key, just display an error message

get help

ab> !help

then help message should be displayed, and when encoutering invalid command, help message should also be displayed.

quit from the application

ab> !quit

sh> _