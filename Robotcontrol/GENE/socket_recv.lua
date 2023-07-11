require "SocketApi"

local serverName = "SRV"
local acceptSocket = nil
local connection = false
local recvList = {} --��Ϣ���ջ������
--local recvData = nil
--local recvLength = 0
local ReceiveSize = 0
local CHUANCANData = 0

local function read_num(name)
	local databody = GetVarFromSDB(name,"NUMDATA")
	return databody.num
end

local function read_string(name)
	local databody = GetVarFromSDB(name,"STRINGDATA")
	return databody.str
end

local function write_num(name, num)
	local databody = {}
	databody.num = num
	databody.name = name
	return SetVarToSDB(name,"NUMDATA",databody)
end

local function write_string(name, str)
	local databody = {}
	databody.str = str
	databody.name = name
	return SetVarToSDB(name,"STRINGDATA",databody)
end

function socket_init()
	print("socket_init...")
	connection = false
	acceptSocket = nil
	--socketMsg.str = ""
  write_string("socketMsg","")
  --isSocketSend.num = 0
  write_num("isSocketSend",0)
	recvList = {}
	ReceiveSize = 0
	CHUANCANData = 0
end


function socket_server()
	if not createrSocket then
		connection = false
		createrSocket = SocketCreate(serverName,"192.168.89.125",8000)
		if createrSocket then
			print("socket_server:SocketCreate succeed.")
		else
			print(string.format("socket_server:SocketCreate failed, return:%s.", tostring(createrSocket)))
		end
	end
	if createrSocket and connection == false then
		socket_init()
		print("socket_server:Wait for connected...")	
		connection, acceptSocket = SocketAccept(serverName)
		if connection and acceptSocket ~= nil then
			print("socket_server:SocketAccept succeed.")
		else
			print(string.format("socket_server:SocketAccept failed, return:%s,%s.", tostring(connection),acceptSocket))
		end
	end
end

function socket_client()
	if not connection then
		socket_init()
		print("socket_client:Wait for connected...")		
		local retVal = SocketConnect(serverName,"192.168.89.125",8000,100000)
		if retVal == 1 then
			connection = true
			print("socket_client:SocketConnect succeed.")
		else
			connection = false
			print(string.format("socket_client:SocketConnect failed, return:%d.", retVal))
		end
	end
end

function send_cmd_data()
    local isSend = -1
	isSend = read_num("isSocketSend")
	print(string.format("send_cmd_data:read_num:isSocketSend=%d, socketMsg = nil %s", isSend, socketMsg.str));
	if isSend == 0 then
		if #recvList > 0 then
  			local str = recvList[1]
  			table.remove(recvList,1)
  			write_string("socketMsg",str)
  			CHUANCANData = CHUANCANData +1
  			write_num("isSocketSend",1)
  			print(string.format("send_cmd_data:push size:%d ,list size:%d, current str:%s, data:%s", CHUANCANData, #recvList, str, socketMsg.str))
  		end
	else
		print(string.format("send_cmd_data:isSocketSend:%d, socketMsg:%s", isSend, socketMsg.str))
	end
end


function socket_receive_loop()
	print("socket_receive_loop:enter socket client loop...")
	local createrSocket = false
	while true do	
		print(string.format("socket_server begin."))
		socket_server()	
		if connection and acceptSocket ~= nil then
			print(string.format("socket_receive_loop:houtai begin."))
			local retVal = -1
			local recvData = nil
			local recvLength = 0
			retVal, recvData, recvLength = SocketReceive(acceptSocket,0,40)
			print(string.format("socket_receive_loop:ret:%s,recvData:%s,recvLength:%s", tostring(retVal), tostring(recvData),tostring(recvLength)))
		
			if retVal == 1 then
				if recvData ~= nil and recvData ~= "" then
					table.insert(recvList,recvData)
					ReceiveSize = ReceiveSize + 1
					print(string.format("socket_receive_loop:ReceiveSize:%d,index:%d,recvData: %s",ReceiveSize, #recvList,recvData));
				end
			elseif retVal == -11 then
				SocketDisconnect(acceptSocket)
				print(string.format("socket_receive_loop:connect failed:%d.",retVal))
				socket_init()
        		recvList = {}
			elseif retVal == -12 then
				print(string.format("socket_receive_loop:recv timeout."))
			end
				send_cmd_data()
		end
	--	Sleep(10)
	end
end


--socket_client()
socket_server()
print(string.format("socket_receive_loop begin!"))
socket_receive_loop()

