#include "LoginRegisterScene.h"
#include "ui\CocosGUI.h"
#include "network\HttpClient.h"
#include "json\document.h"
#include "Utils.h"

USING_NS_CC;
using namespace cocos2d::network;
using namespace cocos2d::ui;

cocos2d::Scene * LoginRegisterScene::createScene() {
  return LoginRegisterScene::create();
}

bool LoginRegisterScene::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto loginButton = MenuItemFont::create("Login", CC_CALLBACK_1(LoginRegisterScene::loginButtonCallback, this));
  if (loginButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + loginButton->getContentSize().height / 2;
    loginButton->setPosition(Vec2(x, y));
  }

  auto registerButton = MenuItemFont::create("Register", CC_CALLBACK_1(LoginRegisterScene::registerButtonCallback, this));
  if (registerButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + registerButton->getContentSize().height / 2 + 100;
    registerButton->setPosition(Vec2(x, y));
  }

  auto backButton = MenuItemFont::create("Back", [] (Ref* pSender) {
    Director::getInstance()->popScene();
  });
  if (backButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - backButton->getContentSize().height / 2;
    backButton->setPosition(Vec2(x, y));
  }

  auto menu = Menu::create(loginButton, registerButton, backButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  usernameInput = TextField::create("username", "arial", 24);
  if (usernameInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 100.0f;
    usernameInput->setPosition(Vec2(x, y));
    this->addChild(usernameInput, 1);
  }

  passwordInput = TextField::create("password", "arial", 24);
  if (passwordInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 130.0f;
    passwordInput->setPosition(Vec2(x, y));
    this->addChild(passwordInput, 1);
  }

  messageBox = Label::create("", "arial", 30);
  if (messageBox) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 200.0f;
    messageBox->setPosition(Vec2(x, y));
    this->addChild(messageBox, 1);
  }

  return true;
}
//��¼����
void LoginRegisterScene::loginButtonCallback(cocos2d::Ref * pSender) {
  // Your code here
	//��Ӧ�ŷ���˵�username��password������json��ʽ������
	std::string username = usernameInput->getStringValue();
	std::string password = passwordInput->getStringValue();
	std::string postData = "{\"username\":\"" + username + "\"," + "\"password\":\"" + password + "\"}";

	//��post��ʽ����,����ppt�������ʾ����auth
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://127.0.0.1:8000/auth");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginRegisterScene::onHttpRequestCompleted, this));
	request->setRequestData(postData.c_str(), strlen(postData.c_str()));

	//�������ʹ��Cookies�ĵط���ֻҪ����һ��ͺã������ڱ�ͷ����
	//�ڵ�¼��ʱ�����cookies�Ϳ��ԣ���������˾Ϳ��Լ�¼����ʹ�õĿͻ�
	//��Ϊ�л��û��ı�ʶ
	cocos2d::network::HttpClient::getInstance()->enableCookies(NULL);
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

//��¼�Ļص���������Ҫ�����ص�״̬
void LoginRegisterScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response) {
		usernameInput->setString("not 200");
		return;
	}
	auto buffer = response->getResponseData();
	rapidjson::Document doc;
	doc.Parse(buffer->data(),buffer->size());
	if (doc["status"] == true) {
		this->messageBox->setString("Login OK");
	}
	else {
		this->messageBox->setString(std::string("Login Failed\n") + doc["msg"].GetString());
	}
}
//ע�ắ��
void LoginRegisterScene::registerButtonCallback(Ref * pSender) {
  // Your code here
	//�͵�¼�������
	std::string username = usernameInput->getStringValue();
	std::string password = passwordInput->getStringValue();
	std::string postData = "{\"username\":\"" + username + "\"," + "\"password\":\"" + password + "\"}";

	//��Post���䷽ʽ������PPT�������ʾ��users
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://127.0.0.1:8000/users");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginRegisterScene::onHttpRequestCompleted1, this));
	request->setRequestData(postData.c_str(), strlen(postData.c_str()));

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}
//ע��Ļص�����,��Ҫ��ʾ��Ϣ
void LoginRegisterScene::onHttpRequestCompleted1(HttpClient *sender, HttpResponse *response)
{
	auto buffer = response->getResponseData();
	rapidjson::Document doc;
	doc.Parse(buffer->data(), buffer->size());
	if (doc["status"] == true) {
		this->messageBox->setString("Register OK");
	}
	else {
		this->messageBox->setString(std::string("Register Failed\n") + doc["msg"].GetString());
	}
}