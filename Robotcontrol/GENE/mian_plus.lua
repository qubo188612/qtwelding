require "SocketApi"

local ML_num = 0
local MJ_num = 0
local accept_num = 0

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

--????robtarget??-??????
local function createRobTarget(str)
  --if str == nil then
	   --TPWrite(string.format("createRobTarget:nil"))
     --return nil
  --else
	   --TPWrite(string.format("createRobTarget:%s",str))
  --end
	local _,_,x,y,z,rx,ry,rz = string.find(str, "T([+-]?%d+%.?%d*) ([+-]?%d+%.?%d*) ([+-]?%d+%.?%d*) ([+-]?%d+%.?%d*) ([+-]?%d+%.?%d*) ([+-]?%d+%.?%d*)")
	if x ~= nil then
		-- rx,ry,rz in degree
		local x_num = tonumber(x)
		local y_num = tonumber(y)
		local z_num = tonumber(z)
		local rx_num = tonumber(rx)
		local ry_num = tonumber(ry)
		local rz_num = tonumber(rz)
		return CreateRobT(P1,x_num,y_num,z_num,rx_num,ry_num,rz_num)
	else
		return nil
	end
end

--????jointtarget??-?????????
local function createJointTarget(str)
  --if str == nil then
	   --TPWrite(string.format("createJointTarget:nil"))
     --return nil
  --else
	   --TPWrite(string.format("createJointTarget:%s",str))
  --end
	local _,_,r_1,r_2,r_3,r_4,r_5,r_6 = string.find(str, "J([+-]?%d+%.?%d*) ([+-]?%d+%.?%d*) ([+-]?%d+%.?%d*) ([+-]?%d+%.?%d*) ([+-]?%d+%.?%d*) ([+-]?%d+%.?%d*)")
	
	if r_1 ~= nil then
		
		local r1 = tonumber(r_1)
		local r2 = tonumber(r_2)
		local r3=  tonumber(r_3)
		local r4 = tonumber(r_4)
		local r5 = tonumber(r_5)
		local r6 = tonumber(r_6)
		local joinTarget = GetJointTarget(0) 
		joinTarget.robax.rax_1 = r1 
		joinTarget.robax.rax_2 = r2
		joinTarget.robax.rax_3 = r3
		joinTarget.robax.rax_4 = r4
		joinTarget.robax.rax_5 = r5
		joinTarget.robax.rax_6 = r6
		--joinTarget.extax,rax_7 = 0
		--joinTarget,extax.eax 1 = 0
		--ioinTarget.extax,eax_2 = 0
		--joinTarget.extax.eax_3 = 0
		--joinTarget.extax.eax 4 = 0
		--joinTarget.extax.eax_5 = 0
		--joinTarget.extax.eax_6 = 0
		--joinTarget.extax.eax 7 = 0
		return joinTarget
	else
		return nil
	end
end

function execute_MoveL(robTarget)
	if robTarget ~= nil then
			-- shareTool0,shareWobj0 should modified by your settings.
			ConfLOff()
			MoveL(robTarget,v10,z1,TCP0405,wobj0,load0)
			ML_num = ML_num + 1
			--TPWrite(string.format("ML_num: %s",ML_num))
		else
			TPWrite("Get a nil robTarget")
		end
end

function execute_MoveAbsJ(jointTarget)
if jointTarget ~= nil then
			-- shareTool0,shareWobj0 should modified by your settings.
			MoveAbsJ(jointTarget,v120,z1,TCP0330,wobj0,load0)
			MJ_num = MJ_num + 1
  	  print(string.format("MJ_num: %d.", MJ_num))
			--TPWrite(string.format("MJ_num: %s",MJ_num))
		else
			TPWrite("Get a nil jointTarget")
		end
end


function execute_ArcLStart(ArcLStartTarget)
    if ArcLStartTarget ~= nil then
			-- shareTool0,shareWobj0 should modified by your settings.
			ArcLStart(ArcLStartTarget,v20,z0,TCP,wobj0,load0)
		else
			TPWrite("Get a nil ArcLStartTarget")
		end
end


function execute_ArcL(ArcLTarget)
if ArcLTarget ~= nil then
			-- shareTool0,shareWobj0 should modified by your settings.
			ArcL(ArcLTarget,v20,z0,TCP,wobj0,load0)
		else
			TPWrite("Get a nil ArcLTarget")
		end
end


function execute_ArcLEnd(ArcLEndTarget)
if ArcLEndTarget ~= nil then
			-- shareTool0,shareWobj0 should modified by your settings.
			ArcLEnd(ArcLEndTarget,v20,z0,TCP,wobj0,load0)
		else
			TPWrite("Get a nil ArcLEndTarget")
		end
end

--???????????sendMsg??????????????????????????sendMsg???????????
function socket_receive_parse()
     local isRecvData = -1
     isRecvData = read_num("isSocketSend")
     print(string.format("socket_receive_parse start:isRecvData:%d",isRecvData))
     if isRecvData == 1 then
         local str = read_string("socketMsg")
  	     local cmd, para = string.match(str,"(.+)#(.+)")
         accept_num = accept_num + 1 
         print(string.format("socket_receive_parse start:accept_num :%s,current recevice cmd:%s, data:%s",accept_num, cmd, para))
  	     if cmd == "MoveL" then
    			   local robTarget = createRobTarget(para)
             execute_MoveL(robTarget)
    		elseif cmd == "MoveAbsJ" then
    			  local jointTarget = createJointTarget(para)
    			  execute_MoveAbsJ(jointTarget)
    		elseif cmd == "ArcLStart" then
    		    local robTarget = createRobTarget(para)
    		    execute_ArcLStart(robTarget)
    		elseif cmd == "ArcL" then
    		    local robTarget = createRobTarget(para)
    		    execute_ArcL(robTarget)
    		elseif cmd == "ArcLEnd" then
    		    local robTarget = createRobTarget(para)
    		    execute_ArcLEnd(robTarget)
    		elseif cmd == "MJ_num" then
    		    TPWrite(string.format("MJ_num: %s",MJ_num))
    		    MJ_num = 0
    			  accept_num = 0
    		elseif cmd == "ML_num" then
    		    TPWrite(string.format("ML_num: %s",ML_num))
    		    ML_num = 0
    		else 
    			  socketMsg.str = ""
    		end
 			  write_string("socketMsg","")
		    write_num("isSocketSend",0)
        print(string.format("socket_receive_parse end:isSocketSend:%d,socketMsg:%s",isSocketSend.num, socketMsg.str))
      end
  	
    Sleep(10)
  
end
--cmd ??server????????h?????????????????????????
--para ??????????????????????
--sendMsg???????????????????????
--??????????????sendMsg???????????????????
function socket_cmd()
	accept_num = 0
	while true do
		socket_receive_parse()
	end
end

socket_cmd()
 
local function GLOBALDATA_DEFINE()
ROBTARGET("P1",{0.000,0.000,0.000},{1.000000,0.000000,0.000000,0.000000},{0,0,0,0},{0.000,0.000,0.000,0.000,0.000,0.000,0.000},0.000)
end
print("The end!")