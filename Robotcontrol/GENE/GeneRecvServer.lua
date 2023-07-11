--文件等级：一般
--author:klug
--献给美人儿雷梅黛丝
--note:展厅智仓机器人，缓存运动数据
--start:230612
--end:230612

require "SocketApi"

local ServerName_recv = "sunny_robot_recv" --发送服务器名称
local Client_recv = "" --accept接收的客户端
local ret = false --反映socket状态
local createflag_recv = false --反映接收socket状态
local ServerIp = "192.168.1.30" --服务器IP
local DataPort_recv = 30010 --服务器接受数据端口
local recv_size = 0 -- 接收的数据个数
local recv_list = {} --消息接收缓存队列

local ReceiveData = nil
local ReceiveLength = 0
local ConnectBroken_recv = true

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
	if not createflag_recv then
		ret = SocketCreate(ServerName_recv, ServerIp, DataPort_recv) --创建socket 2 端口接受上位机命令
		if not ret then
			TPWrite("Socket recv Create Fail!!!")
			return
		end

		if ret then
			TPWrite("Socket recv Create Success...")
			createflag_recv = true
		end
	end

	if ConnectBroken_recv then
		TPWrite("Wait For Connecting...")

		ret, Client_recv = SocketAccept(ServerName_recv) --阻塞等待客户端连接
		if ret then
			ConnectBroken_recv = false
		end
	end
	
	--接受从上位机发送的数据，位姿，以 ，隔开
	ret, ReceiveData = SocketReceiveString(Client_recv, 5000)
	if ret == 1 and ReceiveData ~= nil then
		write_string("socket_msg",ReceiveData);
	end
end

--local function GLOBALDATA_DEFINE()
--ROBTARGET("Point",{0,0,0},{1,0,0,0},{0,0,0,0},{0.000,0.000,0.000,0.000,0.000,0.000,0.000},0.000)
--JOINTTARGET("Joint",{0,0,0,-0,49.445,-0.001,-0},{0,0,0,0,0,0,0})
--end

print("The End...")