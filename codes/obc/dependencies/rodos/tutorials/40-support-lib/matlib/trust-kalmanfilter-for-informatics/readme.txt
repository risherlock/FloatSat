

Benutzung von 
	"LowpassFilter"
	"Kalmanfilter-fuer-Informatiker" (trust.h)

Schätzung von Lage (NadirVector) beim Hexacopter
      (nur roll und pitch, yaw wird nicht gesucht)

Für Berechnungen ist alles normiert 
	-1 .. 1 
oder 
	0 ..1

jedes .cpp Programm  generiert in matlab (octave) format für Visualisierung.
Die Input/meassurments (Rawdata) ist ebenfalls als matlab Programm kodiert für Visualisierung

input data:
	measurements.m

Compilescript
  	doit


Basis "lib":
	trusted.h		Die Basis für "Kalman für Informatiker"
	lowpassfilter.h 	lowpassfilmer mit noise meassurment
	rawsensors.h		liest sensor data von meassurments.m
	filteredsensors.*	benutzt rawsensors, liest und filter mit lowpassfilter
	matlib.h          	vector3d, quaternion, etc etc

Programm:
	nadirfinder.cpp
		braucht:
			filteredsensors.cpp
			matlib.cpp
	
	example-* : Untermengen von nadirfinder





