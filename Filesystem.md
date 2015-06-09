# File management in omegalib #
<p><b>Last revision:</b> ver. 3.0 - 24 August 2012<br>
</p>

When loading textures, models, application scripts and more, omegalib will have to interface with the target machine file system. We want file paths to be as independent as possible from the filesystem structure and omegalib installation directory. To achieve this, we use the file management functionality available from the omicron library. This functionality allows us to convert relative paths and file names to full paths, and open file streams in a location-independent way.

The main class you will use as a developer is the [DataManager](http://omegalib.googlecode.com/svn/refdocs/trunk/html/classomicron_1_1_data_manager.html) class.

## Converting file paths ##
The most important function of `DataManager` is shown in the following short example:
```
	String relativePath = "myApplication/myFile.txt";
	String fullPath;
	if(DataManager::findFile( relativePath , fullPath))
	{
		// File found
	}
	else
	{
		// File not found
	}
```

`DataManager::findFile` will try to locate `myApplication/myFile.txt`. If found, the method will return the full, OS-dependent file path in the `fullPath` variable. you can then open the file stream using standard library functions or pass it to other libraries. But how does DataManager find the file? omegalib provides data manager with a set of predefined search paths for the file:
  * the current working dir
  * the path of the `data` folder inside the omegalib source directory
  * the path to the omegalib apps root directory, if the omegalib-apps repository is installed (see [the building page](Building.md), CMake options section for more information)

### An example ###
Now, assume we are looking for `myApplication/myFile.txt`, and we are in the following environment:
  * we are running our application from `/home/me/bin`
  * the omegalib source path is `/home/me/omegalib-source`
  * we installed the omegalib apps repository in `/home/me/omegalib-apps`

omegalib will look for the following file (and return the first successfull result):
  * `/home/me/bin/myApplication/myFile.txt`
  * `/home/me/omegalib-source/data/myApplication/myFile.txt`
  * `/home/me/omegalib-apps/myApplication/myFile.txt`

omegalib file lookups always use DataManager. So, whenever you pass a file path to some omegalib method, omegalib will perform search for you. Of course you can also specify absolute file paths if you want to override the search functionality, but in general this is not recommended.

### Changing the default data path ###
omegalib will always search for files in the current working dir and in the omegalib apps repository dir (if available). But you can change the path to the default **omegalib data directory** if needed. All omegalib applications support a `-D` command line options to redirect the data path to a different directory.

For more information see the [command line](CommandLine.md) page.