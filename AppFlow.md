<p><b>Last revision:</b> ver. 3.1 - 29 October 2012<font color='#ff0000'> (This is a preliminary / work in progress document and may be incomplete)</font></p>

# Application Flow #

<p align='right'><img src='http://omegalib.googlecode.com/svn/wiki/appFlow/appFlow.PNG' width='500' /></p><p align='middle'>
<i><sup>An overview of the omegalib application flow.</sup></i>
</p>

## Render Loop ##
What follows is an example call stack to a render pass draw function, indicating the main purpose of functions in the call stack:
  * `RenderPass::render`: rendering happens here. Custom applications and frontends (omegaOsg and omegaVtk) reimplement this module.
  * `Renderer::innerDraw`:  iterate render passes and invoke draw functions
  * `Renderer::draw`: setup per-context resources and cameras
  * `ChannelImpl::frameDraw` set up omegalib draw context
  * `eq::Channel::_cmdFrameDraw`, `co::CommandFunc::operator`, `co::Command::invoke` (Equalizer) draw command dispatch
  * `eq::Pipe::_runThread` **main render loop**
  * `co::base::Thread::_runChild`, `co::base::Thread::runChild` render loop initialization
  * `pthread threadStart` thread entry point

## Application update loop ##
This is an example call stack to an engine module update function. Event handling follows a similar call structure.
  * `Module::update`: update happens here
  * `ModuleServices::update`: interate modules and invoke update functions
  * `Engine::update`:  perform update of internal omegalib data, update interpreter state, update sound environment
  * `ConfigImpl::startFrame`: handle event dispatch and global timer
  * `EqualizerDisplaySystem::run`: **application main loop**
  * `SystemManager::run`: initialize all omegalib facilities
  * `omain`: omegalib main function. Process command line arguments and setup system manager.
  * `main`