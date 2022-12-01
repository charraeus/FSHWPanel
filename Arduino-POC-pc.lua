-- Testprogramm
print("Hallo Welt")

local port = "/dev/tty.usbmodem1461"
local portMode = "r
local arduino = assert(io.open(port, portMode))

if arduino ~= nil then
  print("Arduino gefunden.")
  while arduino ~= nil do
    local line = arduino.read()
    print(line)
  end
  arduino.close()
else
  print("Port nicht gefunden!!")
end
