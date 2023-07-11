--文件等级：一般
--author:klug
--献给美人儿雷梅黛丝
--note:展厅智仓机器人，缓存运动数据
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

--receive from up-machine
local command = ""
local d1_ = 0
local d2_ = 0
local d3_ = 0
local d4_ = 0
local d5_ = 0
local d6_ = 0
local d7_ = 0
local robot_tool = ""
local connect_flag = 0;

--ClkStart("clock10") --时钟打开

--从SDB中读取数据
local function read_num(name)
	local databody = GetVarFromSDB(name,"NUMDATA")
	return databody.num
end

--从SDB中读取数据
local function read_string(name)
	local databody = GetVarFromSDB(name,"STRINGDATA")
	return databody.str
end

--向SDB中写入数据
local function write_num(name, num)
	local databody = {}
	databody.num = num
	databody.name = name
	return SetVarToSDB(name,"NUMDATA",databody)
end

--向SDB中写入数据
local function write_string(name, str)
	local databody = {}
	databody.str = str
	databody.name = name
	return SetVarToSDB(name,"STRINGDATA",databody)
end

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
			TPWrite("Connect successfully")
		else
			TPWrite("Connect failed")
			Sleep(1)
		end
	end

	ret, receivedData = SocketReceive(socketName, 0, 5000)
	if ret == 1 and receivedData ~= nil and receivedData ~= "" then
		write_string("socket_msg",ReceiveData);
	else
		connect_flag = 0
	end
end

--local function GLOBALDATA_DEFINE()
--ROBTARGET("Point",{0,0,0},{1,0,0,0},{0,0,0,0},{0.000,0.000,0.000,0.000,0.000,0.000,0.000},0.000)
--JOINTTARGET("Joint",{0,0,0,-0,49.445,-0.001,-0},{0,0,0,0,0,0,0})
--end

print("The End...")