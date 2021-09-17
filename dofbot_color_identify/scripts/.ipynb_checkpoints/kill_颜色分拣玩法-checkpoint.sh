#! /bin/bash
  
len1=` ps -ef|grep 颜色分拣玩法.py |grep -v grep| wc -l`
echo "Number of processes="$len1

if [ $len1 -eq 0 ]
then
    echo "颜色分拣玩法.py is not running "
else
    camera_pid=` ps -ef| grep 颜色分拣玩法.py| grep -v grep| awk '{print $2}'`
    kill -9 $camera_pid
    echo "颜色分拣玩法.py is running, and kill it:"
    echo $camera_pid
fi
sleep 1


len1=` lsof -i:8099 |grep -v grep| wc -l`
echo "Number of processes="$len1

if [ $len1 -eq 0 ]
then
    echo "lsof -i:8099 is not exist "
else
    camera_pid=` lsof -i:8099 | grep -v grep| awk '{print $2}'`
    kill -9 $camera_pid
    echo "lsof -i:8099lsof -i:8099 is running, and kill it:"
    echo $camera_pid
fi
sleep 1