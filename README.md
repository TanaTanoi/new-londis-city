# new-londis-city

Procedural City Generation Project

* Hannah Craighead - Road Map creation
* Tana Tanoi - Building Generation
* Cameron Bryers - Traffic Simulation

# How to use 

##MODES: Specified by the first argument

####Default: [no arguments]
Fully integrated mode

####Showcase mode:
  `showcase [type: int]  [size: int]`
  Displays a single building of a given type
Type options:
 * 0 : 3 Tier
 * 1 : Cut edges L-System
 * 2 : N-Sided Shape L-System
 * 3 : Combined Shape  L-System
 * 4 : Standard L-System
 * 5 : Combined Shape L-System
 * 6 : Modern Style Building
 * 7 : Park

####Car mode:
  `car [*num of cars: int]`
  Displays cars on roads with an optional paramter for number of cars
  
####Section mode:
  `section`
  Displays a randomly generated lot in 2D and how it would be subdivided.
  Pressing n will generate a new lot and pressing s will toggle the wireframe
  and fill mode for drawing the sections.  

####Network mode:
  `network [type: int] [size: int] [cycle bool: int]`
  Displays a road network in 2D TODO define types and cycle

* type: 
	*	`0` - grid
	*	`1` - branch
* size: 
	* `0` - small
	* `1` - medium
	* `2` - large
* cycle:
   * `0` - don't highlight lots
   * `1` - highlight lots

  
##PARAMETERS: Global parameters that change the result of the program
`-seed [seed: string]`
  Change the seed of the program
  
`-size[city_size: int]`
  Change the scale of the city size. Ranges between 0 and 2 as outlined in the network mode
  
`-heightmap`
  Enables the heightmap on launch
  
`-fullscreen`
  Sets fullscreen by default
  
`-joystick`
  enables XBOX controller to be used, if plugged in
  
`-modelview`
  Enables Model View instead of FPS mode for the camera
  
`-fullbright`
  Enables bright ambient lighting over the entire world.
  
`-totalcars [number of cars: int; default 10]`
  Allows more/less cars to be added to the simulation.
  
`-deviance [deviance level: int; default 1]`
  Allows the roads in the system to be more random and less grid like.
  
  * `0` : 0 degree range
  * `1` : -15 to 15 degree range
  * `2` : -30 to 30 degree range
  
`-experimental`
  Uses the branching L-system for the road generation. Uses grid by default
  
