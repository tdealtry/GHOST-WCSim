#!/bin/bash

if ! command -v dialog &> /dev/null
then
	
	if [ "$1" == "" ]
	then
	echo -e "Usage: ./Import <\"Toolname\" or \"All\">"
	elif [ "$1" == "All" ]
	then
	for Tool in `ls -d */|sed s:/:: | grep -v docs | grep -v template`
	do
		ln -s `pwd`/$Tool ../../InactiveTools/$Tool
		if [ -f `pwd`/$Tool/Import.sh ]; then
			echo "Calling Tool $Tool import script..."
			`pwd`/$Tool/Import.sh
			if [ $? -ne 0 ]; then
				echo "Error occurred calling `pwd`/${Tool}Import.sh!"
			fi
		fi
	done
	else
	test=0
	for Tool in `ls -d */ | grep -v docs | grep -v template`
	do
		if [ "$1/" == "$Tool" ]
		then
			ln -s `pwd`/$1 ../../InactiveTools/$1
			if [ -f `pwd`/$Tool/Import.sh ]; then
				echo "Calling Tool $Tool import script..."
				`pwd`/$Tool/Import.sh
				if [ $? -ne 0 ]; then
					echo "Error occurred calling `pwd`/${Tool}Import.sh!"
				fi
			fi
		fi
	done
	
	fi
else
	num=0
	dialog --checklist "Select Tools to import with <spacebar>, Enter for OK and ESC for Cancel:" 0 0 0 \
	`for Tool in \`ls -d */|sed s:/:: | grep -v docs | grep -v template\`
	do
		echo "$Tool $num off "
		num=$(expr 1 + $num)
	done` 2> tmptools
	
	if [ $? -eq 0 ]
	then
		clear
		for Tool in `cat tmptools`
		do
		ln -s `pwd`/$Tool ../../InactiveTools/$Tool
		if [ -f `pwd`/$Tool/Import.sh ]; then
			echo "Calling Tool $Tool import script..."
			`pwd`/$Tool/Import.sh
			if [ $? -ne 0 ]; then
				echo "Error occurred calling `pwd`/$Tool/Import.sh!"
			fi
		fi
		echo -e "\e[38;5;226m$Tool Imported  \e[0m"
		done
		rm tmptools
		
	else
		clear
	fi
fi


