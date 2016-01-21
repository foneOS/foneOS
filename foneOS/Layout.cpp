#include "stdafx.h"
#include "Layout.h"

////
//// FoneOSLabel
////
FoneOSLabel::FoneOSLabel()
{
	this->font = DEFAULT_FONT;
}
FoneOSLabel::FoneOSLabel(FoneOSString str, int xPos, int yPos)
{
	this->font = DEFAULT_FONT;
	this->text = str;
	this->x = xPos;
	this->y = yPos;
	this->Create();
}
void FoneOSLabel::Draw(FoneOSScreen * scr)
{
    HardwareManager::GetDisplay()->DrawString(this->text, this->x, this->y, this->font, this->fontSize * 12, this->fgColor, scr->bgColor);
}
void FoneOSLabel::Create()
{
	char* cText = Utils::FoneOSStringToCharArray(this->text);
	FT_Vector dims = Type::GetDimensions(DEFAULT_FONT, cText, this->fontSize * 12);
	free((void*)cText);
	width = dims.x;
	height = dims.y;
}



////
//// FoneOSTitle
////
void FoneOSTitle::Draw(FoneOSScreen * scr)
{
	HardwareManager::GetDisplay()->FillRectangle(this->x, this->y, this->width, this->height, this->bgColor);
	HardwareManager::GetDisplay()->DrawString(this->text, this->x + 2, this->y + 4, 2, this->fgColor, this->bgColor);
}



////
//// FoneOSButton
////
FoneOSButton::FoneOSButton() {}
FoneOSButton::FoneOSButton(FoneOSString caption, int xPosition, int yPosition)
{
	this->text = caption;
	this->x = xPosition;
	this->y = yPosition;

	this->Create();
}
void FoneOSButton::handleTouch(FoneOSPoint p)
{
	HardwareManager::GetDisplay()->FillRectangle(this->x, this->y, this->width + 2, 2, COLOR_WHITE);
	HardwareManager::GetDisplay()->FillRectangle(this->x, this->y, 2, this->height + 2, COLOR_WHITE);
	HardwareManager::GetDisplay()->FillRectangle(this->x + 2, this->y + 2, this->width, this->height, this->fgColor);
	HardwareManager::GetDisplay()->DrawRectangle(this->x + 2, this->y + 2, this->width, this->height, this->bgColor);
	HardwareManager::GetDisplay()->DrawString(this->text, this->x + 10 + 2, this->y + 10 + 2, this->fontSize, this->bgColor, this->fgColor);

	HardwareManager::GetDisplay()->Flush();
	Utils::Delay(125);

	HardwareManager::GetDisplay()->FillRectangle(this->x, this->y + this->height, this->width + 3, 3, COLOR_WHITE);
	HardwareManager::GetDisplay()->FillRectangle(this->x + this->width, this->y, 3, this->height + 3, COLOR_WHITE);

	this->Draw(NULL);
	HardwareManager::GetDisplay()->Flush();

	if (this->onActivate != NULL)
	{
		this->onActivate(this);
	}
}



//i fixed it and hexed tim
void FoneOSButton::Draw(FoneOSScreen * scr)
{
	HardwareManager::GetDisplay()->FillRectangle(this->x + 2, this->y + 2, this->width, this->height, this->fgColor);
	HardwareManager::GetDisplay()->FillRectangle(this->x, this->y, this->width, this->height, this->bgColor);
	HardwareManager::GetDisplay()->DrawRectangle(this->x, this->y, this->width, this->height, this->fgColor);

	HardwareManager::GetDisplay()->DrawString(this->text, this->x + 10, this->y + 10, this->fontSize, this->fgColor, this->bgColor);
}
void FoneOSButton::Create()
{
	char* cText = Utils::FoneOSStringToCharArray(this->text);
	FT_Vector dims = Type::GetDimensions(DEFAULT_FONT, cText, this->fontSize * 12);
	free((void*)cText);
	this->width = dims.x + 20;
	this->height = dims.y + 20;
}


////
//// FoneOSImage
////
void FoneOSImage::Draw(FoneOSScreen * scr)
{
	if (!HardwareManager::GetDisplay()->DrawImage(this->path, this->x, this->y))
	{
		HardwareManager::GetDisplay()->DrawRectangle(this->x, this->y, this->width, this->height, this->fgColor);
		HardwareManager::GetDisplay()->DrawString(STR("Image error"), this->x, this->y, 1, this->fgColor, this->bgColor);
	}
}



////
//// FoneOSKeyboard
////
void FoneOSKeyboard_callback(FoneOSContainer * btn){
	Logging::LogMessage(((FoneOSButton *)btn)->text);
}
FoneOSKeyboard::FoneOSKeyboard()
{
	int screenWidth = HardwareManager::GetDisplay()->GetWidth();
	int screenHeight = HardwareManager::GetDisplay()->GetHeight();
	
	char line1[] = {'q','w','e','r','t','y','u','i','o','p'};
	//FoneOSButton topRow[10];
	int topRowHeight = screenHeight - (screenHeight/9 * 3);
	char line2[] = {'a','s','d','f','g','h','j','k','l'};
	//FoneOSButton middleRow[9];
	int middleRowHeight = screenHeight - (screenHeight/9 * 2);
	char line3[] = {'z','x','c','v','b','n','m'};
	//FoneOSButton bottomRow[7];
	int bottomRowHeight = screenHeight - (screenHeight/9 * 1);
	FoneOSPoint pt1 = FoneOSPoint();
	FoneOSPoint pt2 = FoneOSPoint();
	for(int i = 0; i < 10; i++){
		topRow[i] = FoneOSButton();
		topRow[i].text = STR(line1[i]);
		topRow[i].x = screenWidth/10 * i;
		topRow[i].y = topRowHeight;
		topRow[i].width = screenWidth/10;
		topRow[i].height = screenHeight/9;
		topRow[i].onActivate = FoneOSKeyboard_callback;
	}
	for(int i = 0; i<9; i++){
		middleRow[i] = FoneOSButton();
		middleRow[i].text = STR(line2[i]);
		middleRow[i].x = screenWidth/10 * i + 1*(screenWidth/10/2);
		middleRow[i].y = middleRowHeight;
		middleRow[i].width = screenWidth/10;
		middleRow[i].height = screenHeight/9;
		middleRow[i].onActivate = FoneOSKeyboard_callback;
	}
	for(int i = 0; i<7; i++){
		bottomRow[i] = FoneOSButton();
		bottomRow[i].text = STR(line3[i]);
		bottomRow[i].x = screenWidth/10 * i + 3*(screenWidth/10/2);
		bottomRow[i].y = bottomRowHeight;
		bottomRow[i].width = screenWidth/10 ;
		bottomRow[i].height = screenHeight/9;
		bottomRow[i].onActivate = FoneOSKeyboard_callback;
	}
	
}

void FoneOSKeyboard::handleTouch(FoneOSPoint p)

{
	
	for(int i=0;i<10;i++){
		if (	p.x > this->topRow[i].x &&
    			p.x < (this->topRow[i].x + this->topRow[i].width) &&
    			p.y > this->topRow[i].y &&
    			p.y < (this->topRow[i].y + this->topRow[i].height)){
  				this->topRow[i].handleTouch(p);
  				
    			}
	}
	for(int i=0;i<9;i++){
  		
  		if (	p.x > this->middleRow[i].x &&
    			p.x < (this->middleRow[i].x + this->middleRow[i].width) &&
    			p.y > this->middleRow[i].y &&
    			p.y < (this->middleRow[i].y + this->middleRow[i].height)){
  				this->middleRow[i].handleTouch(p);
  				
    			}
	}
	for(int i=0;i<7;i++){
  		
  		if (	p.x > this->bottomRow[i].x &&
    			p.x < (this->bottomRow[i].x + this->bottomRow[i].width) &&
    			p.y > this->bottomRow[i].y &&
    			p.y < (this->bottomRow[i].y + this->bottomRow[i].height)){
  				this->bottomRow[i].handleTouch(p);
  				
    			}
	}
}
void FoneOSKeyboard::Draw(FoneOSScreen * scr)
{
	int screenWidth = HardwareManager::GetDisplay()->GetWidth();
	int screenHeight = HardwareManager::GetDisplay()->GetHeight();
	
	HardwareManager::GetDisplay()->DrawRectangle(0, screenHeight - (screenHeight/9 * 3) + 10, screenWidth, screenHeight, COLOR_WHITE);
	
	for(int i=0;i<10;i++){
  		this->topRow[i].Draw(scr);
	}
	for(int i=0;i<9;i++){
  		this->middleRow[i].Draw(scr);
	}
	for(int i=0;i<7;i++){
  		this->bottomRow[i].Draw(scr);
	}
	FoneOSButton space = FoneOSButton();
	space.text = " ";
	space.x = screenWidth/10 * 2 + 3*(screenWidth/10/2);
	space.y = screenHeight - (screenHeight/9 * 1);
	space.width = screenWidth/10*5;
	space.height = screenHeight/9;
	space.onActivate.onActivate = FoneOSKeyboard_callback;
	space.Draw(scr);
	
}

////
//// FoneOSContainer
////
void FoneOSContainer::handleTouch(FoneOSPoint p)
{
	
}
void FoneOSContainer::Draw(FoneOSScreen * scr)
{

}
void FoneOSContainer::Create()
{

}

////
//// FoneOSScreen
////
FoneOSScreen::FoneOSScreen()
{
	this->width = HardwareManager::GetDisplay()->GetWidth();
	this->height = HardwareManager::GetDisplay()->GetWidth();
}

void FoneOSScreen::handleTouch(FoneOSPoint p)
{
	if (this->buttons.size() == 0)
	{
		return;
	}
	std::vector<FoneOSButton*> childrenPointer = this->buttons;
	for (std::vector<FoneOSButton*>::iterator it = childrenPointer.begin(); it != childrenPointer.end(); ++it) {
		FoneOSButton * thingy = *it;
		if (
			p.x > thingy->x &&
			p.x < (thingy->x + thingy->width) &&
			p.y > thingy->y &&
			p.y < (thingy->y + thingy->height)
			) {
			thingy->handleTouch(p);
		}
	}
	std::vector<FoneOSKeyboard*> k_childrenPointer = this->keyboards;
	for (std::vector<FoneOSKeyboard*>::iterator it = k_childrenPointer.begin(); it != k_childrenPointer.end(); ++it) {
		FoneOSKeyboard * thingy = *it;
		thingy->handleTouch(p); // keyboards do additional checks, it's ok
	}
}

#define DrawAllOfType(t, a) \
	do { \
		std::vector<t*> pointer = a; \
		for (std::vector<t*>::iterator it = pointer.begin(); it != pointer.end(); ++it) { \
            (*it)->Draw(this); \
		} \
	} while (0)

void FoneOSScreen::Draw()
{
	HardwareManager::GetDisplay()->Clear(this->bgColor);

	DrawAllOfType(FoneOSImage, this->images);
	DrawAllOfType(FoneOSTitle, this->titles);
	DrawAllOfType(FoneOSLabel, this->labels);
	DrawAllOfType(FoneOSButton, this->buttons);
	DrawAllOfType(FoneOSKeyboard, this->keyboards);
}

int count = 0;

// The current layout.
static FoneOSScreen * CurrentLayout;

static FoneOSScreen * LockScreen;
static FoneOSScreen * Launchpad;
static FoneOSScreen * Dialer;
static FoneOSScreen * SMS;
static FoneOSScreen * More;
static FoneOSScreen * Settings;
static FoneOSScreen * Fonts;
static FoneOSScreen * About;

void HandleLaunchpadTap(FoneOSContainer * cont)
{
	FoneOSButton * button = (FoneOSButton *)cont;

	if (button->text == STR("Dialer"))
	{
		//Layout::SetCurrentLayout(Dialer, true);
		AppManager::OpenApp(STR("foneOS.dialer"));
	}
	else if (button->text == STR("SMS"))
	{
		Layout::SetCurrentLayout(SMS, true);
	}
	else if (button->text == STR("More"))
	{
		Layout::SetCurrentLayout(More, true);
	}
	else if (button->text == STR("Open demoapp"))
	{
		AppManager::OpenApp(STR("foneOS.demoApp"));
	}
}

void HandleDialerTap(FoneOSContainer * cont)
{
	FoneOSButton * button = (FoneOSButton *)cont;

	if (button->text != STR("Call") && button->text != STR("D"))
	{
		if (Dialer->labels[0]->text == STR("Enter number..."))
		{
			Dialer->labels[0]->text = STR("");
		}
		Dialer->labels[0]->text += button->text;
	}
	else if (button->text == STR("Call"))
	{

	}
	else if (button->text == STR("D"))
	{
		if (Dialer->labels[0]->text.length() > 0)
		{
			Dialer->labels[0]->text = Dialer->labels[0]->text.substr(0, Dialer->labels[0]->text.length() - 1);
		}
	}

	Layout::Draw();
}

void HandleMoreTap(FoneOSContainer * cont)
{
	FoneOSButton * button = (FoneOSButton *)cont;

	if (button->text == STR("FM Radio"))
	{

	}
	else if (button->text == STR("Settings"))
	{
		Layout::SetCurrentLayout(Settings, true);
	}
}

void HandleSettingsTap(FoneOSContainer * cont)
{
	FoneOSButton * button = (FoneOSButton *)cont;

	if (button->text == STR("Fonts"))
	{
		Layout::SetCurrentLayout(Fonts, true);
	}
	else if (button->text == STR("About"))
	{
		Layout::SetCurrentLayout(About, true);
	}
}

void UnlockPhone(FoneOSContainer * cont)
{
	Core::ClearStack();
	Layout::SetCurrentLayout(Launchpad, false);
}

void LockPhone(FoneOSContainer * cont)
{
	Core::ClearStack();
	Layout::SetCurrentLayout(LockScreen, false);
}

void Layout::Init()
{
	{
		LockScreen = new FoneOSScreen();
		Launchpad = new FoneOSScreen();
		Dialer = new FoneOSScreen();
		SMS = new FoneOSScreen();
		More = new FoneOSScreen();
		Settings = new FoneOSScreen();
		Fonts = new FoneOSScreen();
		About = new FoneOSScreen();
	}
	{
		FoneOSImage * bg = new FoneOSImage();
		bg->path = STR("purple.bmp");
		bg->width = 240;
		bg->height = 320;
		bg->Create();
		LockScreen->images.push_back(bg);

		FoneOSTitle * title = new FoneOSTitle();
		title->text = STR("Phone locked");
		title->Create();
		LockScreen->titles.push_back(title);

		/*FoneOSButton unlock = FoneOSButton(STR("   Unlock   "), 14, 240);
		unlock.onActivate = &UnlockPhone;
		LockScreen.buttons.push_back(unlock);*/
	}
	{
		FoneOSTitle * title = new FoneOSTitle();
		title->text = STR("Launchpad");
		title->Create();
		Launchpad->titles.push_back(title);

		FoneOSButton * dialer = new FoneOSButton(STR("Dialer"), 5, 30);
		dialer->onActivate = &HandleLaunchpadTap;
		Launchpad->buttons.push_back(dialer);

		FoneOSButton * sms = new FoneOSButton(STR("SMS"), 5, 75);
		sms->onActivate = &HandleLaunchpadTap;
		Launchpad->buttons.push_back(sms);

		FoneOSButton * more = new FoneOSButton(STR("More"), 5, 120);
		more->onActivate = &HandleLaunchpadTap;
		Launchpad->buttons.push_back(more);

		FoneOSButton * demoApp = new FoneOSButton(STR("Open demoapp"), 5, 165);
		demoApp->onActivate = &HandleLaunchpadTap;
		Launchpad->buttons.push_back(demoApp);

		FoneOSButton * lock = new FoneOSButton(STR("Lock phone"), 5, 240);
		lock->onActivate = &LockPhone;
		Launchpad->buttons.push_back(lock);
	}
	{
		FoneOSTitle * title = new FoneOSTitle();
		title->text = STR("Dialer");
		title->Create();
		Dialer->titles.push_back(title);

		FoneOSLabel * number = new FoneOSLabel();
		number->x = 0;
		number->y = 30;
		number->text = STR("Enter number...");
		number->fontSize = 2;
		Dialer->labels.push_back(number);

		int dialerOffsetX = 35;
		int dialerOffsetY = 60;

		FoneOSButton * no1 = new FoneOSButton(STR("1"), dialerOffsetX, dialerOffsetY);
		FoneOSButton * no2 = new FoneOSButton(STR("2"), dialerOffsetX + 60, dialerOffsetY);
		FoneOSButton * no3 = new FoneOSButton(STR("3"), dialerOffsetX + 120, dialerOffsetY);

		FoneOSButton * no4 = new FoneOSButton(STR("4"), dialerOffsetX, dialerOffsetY + 45);
		FoneOSButton * no5 = new FoneOSButton(STR("5"), dialerOffsetX + 60, dialerOffsetY + 45);
		FoneOSButton * no6 = new FoneOSButton(STR("6"), dialerOffsetX + 120, dialerOffsetY + 45);

		FoneOSButton * no7 = new FoneOSButton(STR("7"), dialerOffsetX, dialerOffsetY + 90);
		FoneOSButton * no8 = new FoneOSButton(STR("8"), dialerOffsetX + 60, dialerOffsetY + 90);
		FoneOSButton * no9 = new FoneOSButton(STR("9"), dialerOffsetX + 120, dialerOffsetY + 90);

		FoneOSButton * starBtn = new FoneOSButton(STR("*"), dialerOffsetX, dialerOffsetY + 135);
		FoneOSButton * no0 = new FoneOSButton(STR("0"), dialerOffsetX + 60, dialerOffsetY + 135);
		FoneOSButton * poundBtn = new FoneOSButton(STR("#"), dialerOffsetX + 120, dialerOffsetY + 135);

		FoneOSButton * callBtn = new FoneOSButton(STR("Call"), dialerOffsetX, dialerOffsetY + 180);
		FoneOSButton * delBtn = new FoneOSButton(STR("D"), dialerOffsetX + 120, dialerOffsetY + 180);

		callBtn->width = 96;

		no1->onActivate = no2->onActivate = no3->onActivate = &HandleDialerTap;
		no4->onActivate = no5->onActivate = no6->onActivate = &HandleDialerTap;
		no7->onActivate = no8->onActivate = no9->onActivate = &HandleDialerTap;
		starBtn->onActivate = no0->onActivate = poundBtn->onActivate = &HandleDialerTap;
		callBtn->onActivate = delBtn->onActivate = &HandleDialerTap;

		Dialer->buttons.push_back(no1);
		Dialer->buttons.push_back(no2);
		Dialer->buttons.push_back(no3);

		Dialer->buttons.push_back(no4);
		Dialer->buttons.push_back(no5);
		Dialer->buttons.push_back(no6);

		Dialer->buttons.push_back(no7);
		Dialer->buttons.push_back(no8);
		Dialer->buttons.push_back(no9);

		Dialer->buttons.push_back(starBtn);
		Dialer->buttons.push_back(no0);
		Dialer->buttons.push_back(poundBtn);

		Dialer->buttons.push_back(callBtn);
		Dialer->buttons.push_back(delBtn);
	}
	{
		FoneOSTitle * title = new FoneOSTitle();
		title->text = STR("SMS");
		title->Create();
		SMS->titles.push_back(title);
	}
	{
		FoneOSTitle * title = new FoneOSTitle();
		title->text = STR("More");
		title->Create();
		More->titles.push_back(title);

		FoneOSButton * fmRadio = new FoneOSButton(STR("FM Radio"), 5, 30);
		fmRadio->onActivate = &HandleMoreTap;
		More->buttons.push_back(fmRadio);

		FoneOSButton * settings = new FoneOSButton(STR("Settings"), 5, 75);
		settings->onActivate = &HandleMoreTap;
		More->buttons.push_back(settings);
	}
	{
		FoneOSTitle * title = new FoneOSTitle();
		title->text = STR("Settings");
		title->Create();
		Settings->titles.push_back(title);

		FoneOSButton * fonts = new FoneOSButton(STR("Fonts"), 5, 30);
		fonts->onActivate = &HandleSettingsTap;
		Settings->buttons.push_back(fonts);

		FoneOSButton * storage = new FoneOSButton(STR("Storage"), 5, 75);
		storage->onActivate = &HandleSettingsTap;
		Settings->buttons.push_back(storage);

		FoneOSButton * about = new FoneOSButton(STR("About"), 5, 120);
		about->onActivate = &HandleSettingsTap;
		Settings->buttons.push_back(about);
	}
	{
		FoneOSTitle * title = new FoneOSTitle();
		title->text = STR("Fonts");
		title->Create();
		Fonts->titles.push_back(title);

#ifdef WINDOWS
		// TODO: portability
		WIN32_FIND_DATA findFileData;
		HANDLE search = FindFirstFile(Storage::GetFullPath(STR("fonts\\*")).c_str(), &findFileData);
		int fontNum = 0;
		do
		{
			if (FoneOSString(findFileData.cFileName) != STR(".") && FoneOSString(findFileData.cFileName) != STR("..") && FoneOSString(findFileData.cFileName) != STR("LICENSE.md"))
			{
				FoneOSLabel * fontLabel = new FoneOSLabel();
				fontLabel->text = FoneOSString(findFileData.cFileName);
				fontLabel->x = 5;
				fontLabel->y = (fontNum * 25) + 30;
				fontLabel->font = { FoneOSString(findFileData.cFileName), STR("Regular") };
				fontLabel->Create();
				Fonts->labels.push_back(fontLabel);

				fontNum++;
			}
		} while (FindNextFile(search, &findFileData));
#else
		struct dirent ** searchDir;
		int entries = scandir(Storage::GetFullPath(STR("fonts/")).c_str(), &searchDir, 0, alphasort);
		int fontNum = 0;
		int i;

		for (i = 0; i < entries; i++)
		{
			// TODO: Unify this with the Windows version.

			if (FoneOSString(searchDir[i]->d_name) != STR(".") && FoneOSString(searchDir[i]->d_name) != STR("..") && FoneOSString(searchDir[i]->d_name) != STR("LICENSE.md"))
			{
				FoneOSLabel * fontLabel = new FoneOSLabel();
				fontLabel->text = FoneOSString(searchDir[i]->d_name);
				fontLabel->x = 5;
				fontLabel->y = (fontNum * 25) + 30;
				fontLabel->font = { FoneOSString(searchDir[i]->d_name), STR("Regular") };
				fontLabel->Create();
				Fonts->labels.push_back(fontLabel);

				fontNum++;
				free(searchDir[i]);
			}

		}
		free(searchDir);
#endif
	}
	{
		FoneOSTitle * title = new FoneOSTitle();
		title->text = STR("About fone");
		title->Create();
		About->titles.push_back(title);

		FoneOSLabel * info = new FoneOSLabel();
		info->text = FoneOSString(STR("foneOS\n") + FoneOSString(VERSION));
		info->x = 5;
		info->y = 30;
		About->labels.push_back(info);

		FoneOSString deviceInfoStr;
		deviceInfoStr += STR("Device");
		deviceInfoStr += STR("\n   Serial number: ");
		deviceInfoStr += Device::GetSerialNumber();

		deviceInfoStr += STR("\n");

		deviceInfoStr += STR("\nCPU\n   ");
		deviceInfoStr += Device::GetCPUName();
		deviceInfoStr += STR("\n   CPU SN: ");
		deviceInfoStr += Device::GetCPUSerialNumber();
		deviceInfoStr += STR("\n   Architecture: ");
		deviceInfoStr += Device::GetCPUArchitecture();

		deviceInfoStr += STR("\n");

		deviceInfoStr += STR("\nModem");
		deviceInfoStr += STR("\n   FW rev: ");
		deviceInfoStr += HardwareManager::GetModem()->GetFWRev();
		deviceInfoStr += STR("\n   IMEI: ");
		deviceInfoStr += HardwareManager::GetModem()->GetIMEI();
		deviceInfoStr += STR("\n   CCID: ");
		deviceInfoStr += HardwareManager::GetModem()->GetCCID();

		FoneOSLabel * deviceInfo = new FoneOSLabel();
		deviceInfo->text = deviceInfoStr;
		deviceInfo->x = 5;
		deviceInfo->y = 70;
		deviceInfo->fontSize = 1;
		About->labels.push_back(deviceInfo);
	}

	Layout::SetCurrentLayout(LockScreen, false);
}

void Layout::Draw()
{
	CurrentLayout->Draw();
	Layout::DrawActionBar();
	HardwareManager::GetDisplay()->Flush();
}

void Layout::Update()
{
	FoneOSPoint p = Input::GetTouch();
	while (p.z > 0)
	{
		if (CurrentLayout == LockScreen)
		{
			UnlockPhone(NULL);
		}
		else if (p.y < 280)
		{
			CurrentLayout->handleTouch(p);
		}
		else
		{
			if (p.x > 208)
			{
				Logging::LogMessage(STR("MORE"));
			}
			else if (p.x < 38 && !Core::Stack.empty())
			{
				CurrentLayout = Core::Stack.top();
				Core::Stack.pop();
				Layout::Draw();
			}
			else if (CurrentLayout != LockScreen && CurrentLayout != Launchpad)
			{
				Layout::SetCurrentLayout(Launchpad, true);
				Layout::Draw();
			}
		}
		p = Input::GetTouch();
	}
}

void Layout::DrawActionBar()
{
	if (CurrentLayout != LockScreen)
	{
		HardwareManager::GetDisplay()->FillRectangle(0, 280, 240, 40, COLOR_BLACK);
		if (!Core::Stack.empty())
		{
			HardwareManager::GetDisplay()->DrawImage(STR("icons/back.bmp"), 5, 282);
		}
		HardwareManager::GetDisplay()->DrawImage(STR("icons/home.bmp"), 104, 282);
		HardwareManager::GetDisplay()->DrawImage(STR("icons/more.bmp"), 208, 282);
	}
	else
	{
		HardwareManager::GetDisplay()->FillRectangle(0, 305, 240, 15, COLOR_BLACK);
	}
    if (HardwareManager::GetModem()->GetEnabled())
	{
        int rssi = HardwareManager::GetModem()->GetRSSI();
		int rssiPerc = (int)((((double)rssi) / 31) * 100);

		int grade = 0;
		if (rssiPerc == 0) {
			grade = 0;
		}
		else if (rssiPerc < 20) {
			grade = 1;
		}
		else if (rssiPerc < 40) {
			grade = 2;
		}
		else if (rssiPerc < 60) {
			grade = 3;
		}
		else if (rssiPerc < 80) {
			grade = 4;
		}
		else {
			grade = 5;
		}
		HardwareManager::GetDisplay()->DrawImage(FoneOSString(STR("bars/")) + Utils::IntToString(grade) + FoneOSString(STR(".bmp")), 2, 308);
        HardwareManager::GetDisplay()->DrawString(HardwareManager::GetModem()->GetOperator(), 25, 308, 1, COLOR_WHITE, COLOR_BLACK);
	}
	else
	{
		HardwareManager::GetDisplay()->DrawImage(STR("bars/0.bmp"), 2, 308);
		HardwareManager::GetDisplay()->DrawString(STR("Modem error"), 25, 308, 1, COLOR_WHITE, COLOR_BLACK);
	}
}

FoneOSScreen * Layout::GetCurrentLayout()
{
	return CurrentLayout;
}

void Layout::SetCurrentLayout(FoneOSScreen * layout, bool addLastToStack)
{
	if (addLastToStack)
	{
		Core::Stack.push(CurrentLayout);
	}
	CurrentLayout = layout;
	Layout::Draw();
}
