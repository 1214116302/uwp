#include "UsersInfoScene.h"
#include "network\HttpClient.h"
#include "json\document.h"
#include "Utils.h"

using namespace cocos2d::network;
using namespace rapidjson;

cocos2d::Scene * UsersInfoScene::createScene() {
  return UsersInfoScene::create();
}

bool UsersInfoScene::init() {
  if (!Scene::init()) return false;

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto getUserButton = MenuItemFont::create("Get User", CC_CALLBACK_1(UsersInfoScene::getUserButtonCallback, this));
  if (getUserButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + getUserButton->getContentSize().height / 2;
    getUserButton->setPosition(Vec2(x, y));
  }

  auto backButton = MenuItemFont::create("Back", [](Ref* pSender) {
    Director::getInstance()->popScene();
  });
  if (backButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - backButton->getContentSize().height / 2;
    backButton->setPosition(Vec2(x, y));
  }

  auto menu = Menu::create(getUserButton, backButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  limitInput = TextField::create("limit", "arial", 24);
  if (limitInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 100.0f;
    limitInput->setPosition(Vec2(x, y));
    this->addChild(limitInput, 1);
  }

  messageBox = Label::create("", "arial", 30);
  if (messageBox) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height / 2;
    messageBox->setPosition(Vec2(x, y));
    this->addChild(messageBox, 1);
  }

  return true;
}
//�õ��û���Ϣ�ĺ���
void UsersInfoScene::getUserButtonCallback(Ref * pSender) {
  // Your code here
	
	std::string num = limitInput->getStringValue();
	//����PPT�������ʾ����users?limit=�����䷽ʽ��GET
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://127.0.0.1:8000/users?limit=" + num);
	request->setRequestType(HttpRequest::Type::GET);

	request->setResponseCallback(CC_CALLBACK_2(UsersInfoScene::onHttpRequestCompleted, this));

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

//�õ���Ϣ�Ļص���������΢�е㸴��
void UsersInfoScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) 
{
	if (!response) {
		return;
	}
	auto buffer = response->getResponseData();
	rapidjson::Document doc;
	doc.Parse(buffer->data(), buffer->size());
	//������Ҫ��rapidjson���Ϳ����һ�£��õ�doc������ݽṹ
	if (doc["status"] == true) {
		std::string result;//Ϊ�˽��������result��
		for (int i = 0; i < doc["data"].Size(); i++) {//ʹ�÷�ʽ������vector��
			result += "Username : ";
			result += doc["data"][i]["username"].GetString();
			result += "\nDeck : \n";
			for (int j = 0; j < doc["data"][i]["deck"].Size(); j++) {
				for (auto& m : doc["data"][i]["deck"][j].GetObjectW()) {//C++11��׼�ı���
					result == "\n";
					//��ߵ�һ���������ƺ���\n����һ�������ã�����ֱ���ó�����
					result += "  ";
					result += m.name.GetString();
					result += ":";
					//��߰��ս̳�Ӧ����ֱ��m.value.GetInt���ǻ�����룬���Խ�int��ת��string�ٷŵ�result����
					int a = m.value.GetInt();
					CCString* ns = CCString::createWithFormat("%d", a);
					std::string s = ns->_string;
					result += s;
					CCLOG("m.value.GetInt = %d", m.value.GetInt());
					result += "\n";
				}
				result += " ---\n";
			}
			result += "---\n";
		}
		this->messageBox->setString(result);
	}
	else {
		this->messageBox->setString(std::string("Info Failed\n") + doc["msg"].GetString());
	}
}