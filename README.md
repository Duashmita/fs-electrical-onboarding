Presentation: https://canva.link/0mz2z3y2d7i3jtd
by Ashmita Dua,
for LV team

## File structure

```
fs-electrical-onboarding/
├── README.md
├── Onboarding Project.pdf            # MAIN PRESENTATION
└── lv/
    ├── Notes.pdf                      # Notes, i.e. process of it all
    ├── Schematics_RSS.pdf             # Exported RSS SCHEMATICS (Important)
    ├── RSS_Circuit_KiCad/             # RSS circuit KiCad project
    │   ├── template.kicad_pro         # Project file (open this)
    │   ├── template.kicad_sch         # Root schematic
    │   ├── template.kicad_pcb  
    │   ├── MCU_RSS.kicad_sch    
    │   ├── Power_RSS.kicad_sch   
    │   ├── RSS.kicad_sym          
    │   ├── RSS.pretty/               
    │   ├── RSS.3dshapes/              
    │   ├── sym-lib-table
    │   ├── fp-lib-table
    │   ├── design-block-lib-table
    │   ├── report.txt
    │   └── meta/                      # Template icon and info
    ├── Resources/
    │   ├── assets/            
    │   ├── Shutdown_VCU.kicad_sch     # Reference shutdown sheet
    │   └── bspd_from_fs4_schematics/  # Reference designs from FS4
    │       ├── BSPD_prototype/        # BSPD schematic + PCB
    │       └── VCU/                   # VCU schematics (MCU, IMU, shutdown)
    └── fs-components/                
        ├── formula_slug_symbol_library.kicad_sym
        ├── formula_slug_footprint_library.pretty/
        ├── formula_slug_design_blocks_library.kicad_blocks/
        ├── 3d_models/
        └── formulaslug_drawing_sheet.kicad_wks
```
