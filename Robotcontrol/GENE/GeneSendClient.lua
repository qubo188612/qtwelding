--文件等级：一般
--author:klug
--献给美人儿雷梅黛丝
--note:展厅智仓机器人，后台发送位姿数据给上位机
--start:230612
--end:230612

require "SocketApi"

local socketName = "sock1"
local ret = false
local receivedData = nil
local ReceiveData = nil
local ReceiveLength = 0
local SendDate = nil
local ConnectBroken = true
local ServerIp = "192.168.1.11" --服务器IP
local DataPort = 30004 --服务器发送数据端口

--send to up-machine
local CurrentRobotTarget = nil
local CurrentPositionX = 0
local CurrentPositionY = 0
local CurrentPositionZ = 0
local CurrentPostureQ1 = 0
local CurrentPostureQ2 = 0
local CurrentPostureQ3 = 0
local CurrentPostureQ4 = 0
local time_ms = 0
local connect_flag = 0


--ClkStart("clock10") --时钟打开

function split(str, repl)
    local ret = {}
    local pattern = string.format("([^%s]+)", repl)
    string.gsub(str, pattern, function(w)
	table.insert(ret, w)
    end)
    return ret
end

while true do
	if connect_flag == 0 then
		retVal = SocketConnect(socketName, ServerIp, DataPort, 5000)
		if retVal == 1 then
			connect_flag = 1
			--TPWrite("Connect successfully")
		else
			TPWrite("Connect failed")
			Sleep(1)
		end
	end

	CurrentRobotTarget = GetRobTarget(0, sharetool1, wobj0) --读取机械臂当前位姿
	--time_ms = ClkRead("clock10") --读取时间
	CurrentPositionX = CurrentRobotTarget.trans.x
	CurrentPositionY = CurrentRobotTarget.trans.y
	CurrentPositionZ = CurrentRobotTarget.trans.z
	CurrentPostureQ1 = CurrentRobotTarget.rot.q1
	CurrentPostureQ2 = CurrentRobotTarget.rot.q2
	CurrentPostureQ3 = CurrentRobotTarget.rot.q3
	CurrentPostureQ4 = CurrentRobotTarget.rot.q4
	
	--发送数据给上位机，位姿+程序运行时间
	SendData = "x:"..CurrentPositionX..",y:"..CurrentPositionY..",z:"..CurrentPositionZ..",q1:"..CurrentPostureQ1..",q2:"..CurrentPostureQ2..",q3:"..CurrentPostureQ3..",q4:"..CurrentPostureQ4
	ret = SocketSend(Client_send, SendData, 5000)
	if ret == 1 then
		--TPWrite("Socket Send Success...")
	else
		connect_flag = 0
	end
end

local function GLOBALDATA_DEFINE()

end

print("The End...")