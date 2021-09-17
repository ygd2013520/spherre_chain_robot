#!/bin/sh

source /home/dofbot/catkin_ws/devel/setup.bash
cd /home/dofbot/Arm/
python3 /home/dofbot/Arm/YahboomArm.pyc &
echo "waitting"
sleep 75
echo "go on"
sh /home/dofbot/dofbot_ws/src/dofbot_color_identify/scripts/init_script.sh
sleep 3
python3 /home/dofbot/dofbot_ws/src/dofbot_color_identify/scripts/颜色分拣玩法.py
