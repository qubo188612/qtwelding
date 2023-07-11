--文件等级：一般
--author:klug
--献给美人儿雷梅黛丝
--note:展厅智仓机器人，后台总控
--start:221115
--end:230512

require "SocketApi"

local ServerName_sum = "sunny_robot_sum" --发送服务器名称
local Client_sum = "" --accept接收的客户端
local ret = false --反映socket状态
local createflag_sum = false --反映接收socket状态
local ServerIp = "192.168.1.30" --服务器IP
local DataPort_sum = 30007 --服务器接受数据端口

local ReceiveData = nil
local ReceiveLength = 0
local ConnectBroken_sum = true

--receive from up-machine
local command = ""

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
	if not createflag_sum then
		ret = SocketCreate(ServerName_sum, ServerIp, DataPort_sum) --创建socket 2 端口接受上位机命令
		if not ret then
			TPWrite("Socket recv Create Fail!!!")
			return
		end

		if ret then
			TPWrite("Socket recv Create Success...")
			createflag_sum = true
		end
	end

	if ConnectBroken_sum then
		TPWrite("Wait For Connecting...")

		ret, Client_sum = SocketAccept(ServerName_sum) --阻塞等待客户端连接
		if ret then
			ConnectBroken_sum = false
		end
	end
	
	--接受从上位机发送的命令
	ret, ReceiveData = SocketReceiveString(Client_sum, 5000)
	if ret == 1 then
		TPWrite("Received data: " ..ReceiveData)
		command = ReceiveData
		--Stop()
		--SoftDeact(200)
		ret = SocketSend(Client_sum, ReceiveData, 5000)
		Stop()
	end

	if command == "stop" then
		SoftDeact(200)
	end

	if command == "start" then

	end
end

local function GLOBALDATA_DEFINE()

end

print("The End...")