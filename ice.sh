#!/bash/shell

#porgram:
#	This Program can test IceStorm

#history
#2018/9/28	zjh

count="$(ps -u zjh | grep icebox |wc -l)"
make all
if [ $? -ne 0 ];then
	echo "Makefile构建失败！"
else
	if [ ${count} -gt 0  ];then
		echo "IceStorm服务已启动！"
	else
	 	gnome-terminal --title="IceStorm" -x bash -c "icebox --Ice.Config=config.icebox;exec bash"
	fi
	if [ $? -ne 0 ];then
		echo "开启IceStorm服务失败！"
	else
		gnome-terminal --title="Publisher" -x bash -c "./publisher;exec bash"
		if [ $? -ne 0 ];then
		echo "Publisher 开启失败!"
		fi
		gnome-terminal --title="Subcriber" -x bash -c "./subcriber;exec bash"
		if [ $? -ne 0 ];then
		echo "Publisher 开启失败!"
		fi
	fi
fi
