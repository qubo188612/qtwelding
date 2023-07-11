--文件等级：一般
--author:klug
--献给美人儿雷梅黛丝
--note:展厅智仓机器人，接受数据运动，缓存数据
--start:230612
--end:230612

require "SocketApi"

local isRecvData = 0
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
	isRecvData = read_num("isSocketSend")
	if isRecvData ==1 then
		local RecvStr = read_string("socket_msg")
		list_str = split(RecvStr, ",")
		command = list_str[1]
		d1_ = list_str[2]
		d2_ = list_str[3]
		d3_ = list_str[4]
 		d4_ = list_str[5]
		d5_ = list_str[6]
		d6_ = list_str[7]
		d7_ = list_str[8]

		Point.trans.x = tonumber(d1_)
		Point.trans.y = tonumber(d2_)
		Point.trans.z = tonumber(d3_)
		Point.rot.q1 = tonumber(d4_)
		Point.rot.q2 = tonumber(d5_)
		Point.rot.q3 = tonumber(d6_)
		Point.rot.q4 = tonumber(d7_)
		
		if command == "MoveL" then
			MoveL(Point,v100,fine,sharetool1,wobj0,load0)
			TPWrite("operate the MoveL")
		end

		if command == "MoveJ" then
			MoveJ(Point,v100,fine,sharetool1,wobj0,load0)
			TPWrite("operate the MoveJ")
		end
	end
end

local function GLOBALDATA_DEFINE()
ROBTARGET("Point",{0,0,0},{1,0,0,0},{0,0,0,0},{0.000,0.000,0.000,0.000,0.000,0.000,0.000},0.000)
JOINTTARGET("Joint",{0,0,0,-0,49.445,-0.001,-0},{0,0,0,0,0,0,0})
end

print("The End...")