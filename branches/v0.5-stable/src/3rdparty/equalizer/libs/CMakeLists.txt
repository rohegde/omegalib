# Copyright (c) 2010 Daniel Pfeifer <daniel@pfeifer-mail.de>
#               2010 Stefan Eilemann <eile@eyescale.ch>
#               2010 Cedric Stalder <cedric.stalder@gmail.com>
add_subdirectory(collage)
if(OMEGA_USE_DISPLAY_EQUALIZER)
	add_subdirectory(client)
	add_subdirectory(admin)
	add_subdirectory(server)
endif(OMEGA_USE_DISPLAY_EQUALIZER)

