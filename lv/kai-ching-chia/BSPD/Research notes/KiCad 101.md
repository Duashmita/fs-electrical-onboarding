### What it is:
is used for EDA (electronic design automation) and designing PCBs (printed circuit boards)
1. EDA can be used to
	design
	simulate
	analyze
	& manufacture all types of integrated [[Circuits 101|circuits]] (chips) and PCBs
### File types
1. .kicad_pro: main project file (pro = project)
2. .kicad_sch: schematic layout
3. .kicad_pcb: PCB layout
	- sch and pcb are heavily connected together
* VERY important rule: NEVER rename these files
### Libraries
1. Global: read only libraries that come preinstalled
	- these are basically the mandatory mods
2. Project: custom files stored locally. if you are exporting your project folder into another computer, these custom assets will come alongside
	- like exporting a minecraft world with mods. when you download the world, the mods come with
### PCB design workflow
1. Draw the schematic 
	- put symbols and wire them together, all the logic work
2. Assign footprints
	- link the symbol to actual, physical, real life object
	- conversion from symbols to actual components
3. Layout of the PCB
	- transfer all of the schematic data into a PCB editor
	- arrange the layout (copper tracks, physical parts)
4. Run checks
	- use ERC (electrical rules checker) and DRC (design rules checker) to make sure there are no spacing / short circuit errors
5. Export for manufacturing
	- export into a gerber file (basically conversion into the electrical engineering equivalent of g-code)