--文件等级：一般
--author:klug
--献给美人儿雷梅黛丝
--note:展厅智仓机器人，后台发送位姿数据给上位机
--start:230511
--end:230511

require "SocketApi"

local ServerName_send = "sunny_robot_send" --接收服务器名称
local Client_send = "" --accept发送的客户端
local ret = false --反映socket状态
local createflag_send = false --反映发送socket状态
local ServerIp = "192.168.1.30" --服务器IP
local DataPort_send = 30008 --服务器发送数据端口

local ReceiveData = nil
local ReceiveLength = 0
local SendDate
local ConnectBroken_send = true

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
	if not createflag_send then
		ret = SocketCreate(ServerName_send, ServerIp, DataPort_send) --创建socket 1 端口发送机器人位姿
		if not ret then
			TPWrite("Socket send Create Fail!!!")
			return
		end

		if ret then
			TPWrite("Socket send Create Success...")
			createflag_send = true
		end
	end	

	if ConnectBroken_send then
		TPWrite("Wait For Connecting...")

		ret, Client_send = SocketAccept(ServerName_send) --阻塞等待客户端连接
		if ret then
			ConnectBroken_send = false
			TPWrite("Socket send Accept Success...")
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
	end
end

local function GLOBALDATA_DEFINE()

end

print("The End...")