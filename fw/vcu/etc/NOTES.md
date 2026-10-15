# Notes -- Firmware Onboarding Project
(so that I have a place within VSCode to write things down as I work)
(because what is going on)

## 10/08/26
    - created this file :D
    - i've written clamp() and in_range()
    - now trying to figure out how to write the constructor for ETC_Controller class
        - want to understand the class and the use of the objects made from that class so i can write informedly (assuming that's a word)
    - i already found the PIN names from the kicad schematic
        - APPS_1 -- PC_1 | vv
        - APPS_2 -- PC_2 (<-- confirm this again)
    - APPS = "Accelerator Pedal Position Sensor"
    - Glossary says APPS *RETURNS* a voltage... inferring that it's how we receive info on how far the accelerator pedal's been pressed
        - Have to convert this to . . . what?
        - "writing basic firmware using MBed to write a primitive ETC with appropriate pins from the fs-4 VCU schematic"

## 10/13/26
    - Haven't written notes for the last 2-3 work sessions [about 5-6 hours worth] but here's what happened:
        - relearned C++ class/constructor usage (as opposed to C)
        - spent a lot lot lot of time thinking about what part does
        what, why it does it, and the big picture of what i'm coding
            - reading the rulebook
            - handwritten brainstorm stuff
            - skimming linked resources
        - wrote update_state(), added motor_torque_demand as a member
        variable of the class so i can track it
        - (what is going on)
    - now what?
        - my first thought was to test what i've already written
        - how to test update_state() without having written 
        update_implausibilities()?
        - jack said i can maybe borrow a small microcontroller to push on
            - not sure how to do that + have a midterm tomorrow, so 
            holding off on that and instead relying on compiling
            every so often as a rudimentary catch-all before flashing
            onto the car on Thursday (hopefully)

## 10/14/26 and 10/15/26
    - working on update_implausibilities()
    - tried compiling main, found lots of #include errors >> struggled for a bit with including file paths, then jackson suggested i delete the build folder and reconfigure cmake
    - ^^this took a solid hour, but it's successfully reconfigured now
    - 
        