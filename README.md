CoherentUI_Ogre3D
=================

Sample integration of Coherent UI with the Ogre3D graphics engine

Installation (Windows)
=================

Simply edit the `OgreCoherentUI/OgreHome.props` and `Demo/OgreHome.props` files and change the value of the  `OGRE_HOME` User Macro to your OGRE installation directory.
You also need to edit the `OgreCoherentUI/CoherentUI.props` file and change the `COHERENT_UI_HOME` User Macro accordingly.

Build the provided solution and add the following in your `plugins.cfg` file:  
```
Plugin=Plugin_OgreCoherentUI
```
then run the demo.
