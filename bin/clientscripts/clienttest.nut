function onInit()
{
	print("onInit()");
	addChatMessage("Client-Side script started.");
}

function onPulse()
{

}

function onExit()
{
	print("onExit()");
}

function onRender()
{
	local resolution = getResolution();
	drawBox(resolution[0]-200, resolution[1]-50, 200.0, 50.0, 150, 0, 0, 0);
	drawText(resolution[0]-170, resolution[1]-35, format("Vice-Players: %s", getVersion()), 255, 255, 255, 255);
}