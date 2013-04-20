#!/bin/bash

fnReplaceLinksRecusively()
{
	local f="$1/$2"
	local link=$(head -1 $f)
	echo "check $f:"
	
	if [ -f "$1/$link" ];
	then
	    echo "-> $link"
		d=`dirname "$1/$link"`
		n=`basename "$1/$link"`
		fnReplaceLinksRecusively $d $n
		# replace the file by the link
		echo "mv \"$1/$link\" $f"
		rm -f $f
		mv "$1/$link" $f
	else
		echo "-"
	fi
}

fnReplace()
{
	echo "replacing links in " $1
	
	local wd=`pwd`
	cd $1
	
	for f in *.so
	do
		link=$(head -1 $f)
		echo "check $f:"
		
		if [ -f "$link" ];
		then
			echo "- $link"
			#rm -f $f
			#mv "$1/$link" $f
			#echo "OUTPUT_FORMAT(elf32-i386)" > $f
			#echo "INPUT($link)" >> $f
			#rm -f $f
			#ln -s $link $f
			fnReplaceLinksRecusively "." $f
		else
			echo "-"
		fi
	done
	cd $wd
}

# define some paths
sysroot="i686-naothwin32-linux-gnu/sysroot"
usr_lib="$sysroot//usr/lib"
sys_lib="$sysroot/lib"
libc="$usr_lib/libc.so"

echo "replase all the links by the actual libraries"
fnReplace "$1/$usr_lib"
fnReplace "$1/$sys_lib"

echo "put '..' in front of each of three libs"
sed -i 's: \(/lib/libc.so.\|/usr/lib/libc_nonshared.a\|/lib/ld-linux.so.2\): ..\1:g' "$1/$libc"