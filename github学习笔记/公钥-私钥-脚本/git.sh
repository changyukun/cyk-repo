#!/bin/bash

path_home=`dirname $0`   
cd $path_home 
### 下面的ccyykk文件为另一台电脑（a电脑）上ssh-keygen生成的私钥
### 执行此脚本后即可使用yama进行访问了，如 git clone git@yama:changyukun/cyk-source.git
mkdir -p $HOME/.ssh
cp -rf ccyykk $HOME/.ssh
chmod -R 700 $HOME/.ssh/ccyykk
sshconfig=$HOME/.ssh/config

if [ -e $sshconfig ]; then
	if grep -q "Host yama" $sshconfig
	then
		cat $sshconfig
		echo "Error ---> $sshconfig already exit, please setup manual."
	else
		echo "Host yama" >>$sshconfig
		echo "HostName github.com" >>$sshconfig
		echo "User git" >>$sshconfig
		echo "IdentityFile $HOME/.ssh/ccyykk" >>$sshconfig
	fi
else
	echo "Host yama" >>$sshconfig
	echo "HostName github.com" >>$sshconfig
	echo "User git" >>$sshconfig
	echo "IdentityFile $HOME/.ssh/ccyykk" >>$sshconfig
fi

cd -
