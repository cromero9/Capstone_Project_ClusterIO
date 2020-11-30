local blinker_left_on_off = 0
local blinker_left_action
local blinker_left_event = 0

local blinker_right_on_off = 0
local blinker_right_action
local blinker_right_event = 0

local data  = {}

local gBackendChannel = "cluster_backend"
local toggle

function ignition(mapargs)
local ev = mapargs.context_event_data

local ignition =(ev["ignition"])
if (ignition == 1) then
gre.send_event("IGNITION_ON")
end
if (ignition == 0) then
gre.send_event("IGNITION_OFF")
end

end

function CBUpdateEvent(mapargs)
local ev = mapargs.context_event_data

	local speed_rot = (ev["speed"] * (214/200)) - 107
	local rpm_rot = ((ev["rpm"] / 10000) * 49) + 66
	local drive = (ev["drive"])
	local fuel = (ev["fuel"])


	
	--ev["drive"] = 1
	
	data["speedometer.pointer_speedometer.rot"] = speed_rot 
	data["speedometer_content.speed.text"] = tostring(ev["speed"])
	data["tach_exterior.pointer_tach_exterior.rot"] = rpm_rot
	
	--print(fuel)
	if (fuel == 90) then
	data["gas_exterior.full_10.alpha"] = 0
	print("fuck you fuel")
	end
	
	if (fuel == 80) then
  data["gas_exterior.full_9.alpha"] = 0
  end
  
  if (fuel == 70) then
  data["gas_exterior.full_7.alpha"] = 0
  end

	
	gre.set_data(data)
end

function update_drive(mapargs)


gre.send_event_data (
    "drive_selection",
    "2u1 drive", 
    {drive = 1}, 
    gBackendChannel
  )
end

function update_park(mapargs)

gre.send_event_data (
    "park_selection",
    "2u1 park", 
    {drive = 1}, 
    gBackendChannel
  )
end

function change_to_mph(mapargs)


data["speedometer_content.speed_unit.text"] = "MPH"

end

function get_date_time(mapargs)
data["background.time.text"] = os.date()
end

function update_radio1(mapargs)



gre.send_event_data (
    "update_radio",
    "2u1 radio_channel", 
    {radio_channel = 1}, 
    gBackendChannel
  )



end

function update_radio2(mapargs)
local ev = mapargs.context_event_data
local gBackendChannel = "cluster_backend"


gre.send_event_data (
    "update_radio",
    "2u1 radio_channel", 
    {radio_channel = 2}, 
    gBackendChannel
  )



end

function update_radio3(mapargs)


gre.send_event_data (
    "update_radio",
    "2u1 radio_channel", 
    {radio_channel = 3}, 
    gBackendChannel
  )
end

function blinker_left_toggle_on(mapargs)

blink_right_on_off = 0
gre.timer_clear_interval(blinker_right_action)
data["indicators.R_arrow_glow.image"] = ""
gre.set_data(data)
gre.send_event_data (
    "blinker_right",
    "2u1 blinker_right", 
    {blinker_right = 0}, 
    gBackendChannel
  )


if (blinker_left_on_off == 0) then
blinker_left_on_off = 1
blinker_left_action = gre.timer_set_interval(blinker_left,1000)

else
blinker_left_on_off = 0 
gre.timer_clear_interval(blinker_left_action)
blinker_left(mapargs)


end

end

function blinker_left(mapargs)


local gBackendChannel = "cluster_backend"

 
if (blinker_left_on_off == 1) then
print("Blinker enabled")
if (blinker_left_event == 0) then
gre.send_event_data (
    "blinker_left",
    "2u1 blinker_left", 
    {blinker_left = 1}, 
    gBackendChannel
  )
 data["indicators.L_arrow_glow.image"] = "images/L_arrow_glow.png" 
 gre.set_data(data) 
 blinker_left_event = 1
 print("Light on")
else 
data["indicators.L_arrow_glow.image"] = ""
gre.set_data(data)
gre.send_event_data (
    "blinker_left",
    "2u1 blinker_left", 
    {blinker_left = 0}, 
    gBackendChannel
  )
blinker_left_event = 0
print("Light off")
end

else
data["indicators.L_arrow_glow.image"] = ""
gre.set_data(data)
gre.send_event_data (
    "blinker_left",
    "2u1 blinker_left", 
    {blinker_left = 0}, 
    gBackendChannel
  )
print("Blinker disabled")
end 

end

function blinker_right_toggle_on(mapargs)

blink_left_on_off = 0
gre.timer_clear_interval(blinker_left_action)
data["indicators.L_arrow_glow.image"] = ""
gre.set_data(data)
gre.send_event_data (
    "blinker_left",
    "2u1 blinker_left", 
    {blinker_left = 0}, 
    gBackendChannel
  )

if (blinker_right_on_off == 0) then
blinker_right_on_off = 1
blinker_right_action = gre.timer_set_interval(blinker_right,1000)

else
blinker_right_on_off = 0 
gre.timer_clear_interval(blinker_right_action)
blinker_right(mapargs)


end


end

function blinker_right(mapargs)


local gBackendChannel = "cluster_backend"

 
if (blinker_right_on_off == 1) then
print("Blinker enabled")
if (blinker_right_event == 0) then
gre.send_event_data (
    "blinker_right",
    "2u1 blinker_right", 
    {blinker_right = 1}, 
    gBackendChannel
  )
 data["indicators.R_arrow_glow.image"] = "images/R_arrow_glow.png" 
 gre.set_data(data) 
 blinker_right_event = 1
 print("Light on")
else 
data["indicators.R_arrow_glow.image"] = ""
gre.set_data(data)
gre.send_event_data (
    "blinker_right",
    "2u1 blinker_right", 
    {blinker_right = 0}, 
    gBackendChannel
  )
blinker_right_event = 0
print("Light off")
end

else
data["indicators.R_arrow_glow.image"] = ""
gre.set_data(data)
gre.send_event_data (
    "blinker_right",
    "2u1 blinker_right", 
    {blinker_right = 0}, 
    gBackendChannel
  )
print("Blinker disabled")
end 
 
end


