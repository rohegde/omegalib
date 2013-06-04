## omegalib v4.0 
### Notes
This is the current developer version
a different note here

### Major Changes

**omega**
- Added `Color` from omicron. Added `Color.isValidColor`
- Added per-tile runtime camera controls: `isHostInTileSection`, `setTileCamera`.
- `pointerSize` config option
- Automatic multi-instance port adjustment for NetService
- on the `-I` multi-instance switch, the port pool can be left unspecified. It will be set to 100 by default.
**cyclops**
- Major revision of the Material API. Materials have a lot more properties and flags. Multiple material layers can be added to an entity, without having to use the `setEffect` string. `setEffect` is not just a shorthand version of adding material layers to an Entity.
- 4.0.1 **FIX** incorrect handling of `-t` transparent effect flag.
**omicron**
- Improved sound asset management: sound assets now get copied automatically to the sound server.
- **NOTE FOR APPLICATION DEVELOPERS** All sound paths in applications need to switch to use relative paths.
