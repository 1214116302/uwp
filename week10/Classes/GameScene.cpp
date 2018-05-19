#include "GameScene.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	//����ط�дSence::create()���ᱨ��,����û����ʾ����ҳ�档
	return GameSence::create();
}

// on "init" you need to initialize your instance
bool GameSence::init()
{
	
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//���ñ���ͼƬ
	auto levelBackGround = Sprite::create("level-background-0.jpg");
	levelBackGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(levelBackGround, 0);
	//����ʯͷ��
	auto stoneLayer = Layer::create();
	stoneLayer->setPosition(0, 0);
	stoneLayer->setName("stoneLayer");
	this->addChild(stoneLayer, 1);
	//��ʯͷ�����ʯͷ
	auto stone = Sprite::create("stone.png");
	stone->setPosition(600, 480);
	stone->setName("stone");
	stoneLayer->addChild(stone, 1);
	//���������
	auto mouseLayer = Layer::create();
	mouseLayer->setPosition(0, visibleSize.height / 2);
	mouseLayer->setName("mouseLayer");
	this->addChild(mouseLayer, 1);
	//��������������
	//����ط��޿ӣ���Ϊlevel-sheet.plist�ļ�������˲�ֹһ������Ķ���
	//һ��ʼ��������Ķ������������ʯ��һ�������뵽������������������ʯ
	//�����Ļ������������Ƕ��ģ���ʯ�ǲ����ģ�������Ҫ��
	//���԰���ʯ��Ϊ��������࣬������ʯ�Ϳ��Ժ�����һ��
	//������ϸ��demo��ʾ���������Ǳ�����ʯ�ģ�������ʯ�Ǹ��ǵ����ӵģ�������ȫ�����ܵ�
	//��Ϊ������һ������ģ���ʯ��һ������ģ�Ҫô���󸲸���ʯ��Ҫô��ʯ��������
	//�������������ֻ��һ������������ʯ�������һ��ͼƬ����������
	const std::string mouseFrameName = "gem-mouse-0.png";
	auto mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));
	mouse->setPosition(visibleSize.width / 2, 0);
	mouse->setName("mouse");
	mouseLayer->addChild(mouse, 1);

	//����shoot����
	const char *shootName = "shoot";
	auto shootText = Label::createWithTTF(shootName, "fonts/Marker Felt.ttf", 72);
	shootText->setPosition(Vect(origin.x + visibleSize.width - shootText->getContentSize().width, 480));
	shootText->setName("shootText");
	this->addChild(shootText, 1);

	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {
	//�õ�������ĵط�
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto location = touch->getLocation();
	//�ܺõĿ���̨�����䣬��uwp�������
	CCLOG("location == %f, location == %f", location.x, location.y);
	//���ÿ��ʵ���������ַ���getChildByName,����getChildByTag
	//���˾���getChildByName��ֱ��һЩ����Ҫ��ǰ������setName
	//Tag����int�͵ģ���Ҫ��addChild��ʱ������
	auto stoneLayer = this->getChildByName("stoneLayer");
	auto stone = stoneLayer->getChildByName("stone");
	auto mouseLayer = this->getChildByName("mouseLayer");
	auto mouse = mouseLayer->getChildByName("mouse");
	auto shootText = this->getChildByName("shootText");

	//�ж��Ƿ�����shoot����
	bool isInShootLabel = shootText->getBoundingBox().containsPoint(location);
	if (isInShootLabel) {
		//����������һ��stone��Ȼ�������ɵ�stone����
		//stone�ƶ���������������꣬Ȼ����ʧ���õ��Ƕ������У���Ҫ����addChild������û���κη�Ӧ
		auto stoneTempMoveTo = MoveTo::create(0.5, mouseLayer->convertToWorldSpace(mouse->getPosition()));
		auto stoneTemp = Sprite::create("stone.png");
		stoneTemp->setPosition(600, 480);
		stoneLayer->addChild(stoneTemp);
		//ʯͷ������ʧ
		auto stoneTempFadeOut = FadeOut::create(0.5);
		//�����ɵ�stoneɾ��������Լ��Դ
		auto callBackRemove = CallFunc::create([this, &stoneTemp]() {
			this->removeChild(stoneTemp);
		});
		auto stoneTempSequence = Sequence::create(stoneTempMoveTo, stoneTempFadeOut,callBackRemove,nullptr);
		stoneTemp->runAction(stoneTempSequence);

		//�������һ����λ�ã���Ϊ���׳��磬���Ը�ȡ��1/2
		auto mouseNewPosition = Vec2(CCRANDOM_0_1() * visibleSize.width*0.5, CCRANDOM_0_1() * visibleSize.height*0.5);
		auto mouseMoveTo = MoveTo::create(0.5, mouseNewPosition);
		mouse->runAction(mouseMoveTo);
		//��ʯͷ�����������ƶ���������ʯ�������������������ϵ��
		auto diamond = Sprite::create("diamond.png");
		diamond->setPosition(mouseLayer->convertToWorldSpace(mouse->getPosition()));
		this->addChild(diamond);
	}
	else {
		//�������ʱ����shoot���ͻ����cheese��cheeseλ���������λ��
		auto cheese = Sprite::create("cheese.png");
		cheese->setPosition(location);
		this->addChild(cheese);
		//�����ƶ���cheeseλ�ã���Ҫ��cheeseת��mouseLayer���������
		auto mouseMoveTo = MoveTo::create(0.5, mouseLayer->convertToNodeSpace(location));
		mouse->runAction(mouseMoveTo);
		//cheese������ʧ
		auto cheeseFadeOut = FadeOut::create(0.5);
		//ɾ��cheese��ʡ��Դ
		auto callBackRemove = CallFunc::create([this, &cheese]() {
			this->removeChild(cheese);
		});
		auto cheeseSequence = Sequence::create(DelayTime::create(0.5), cheeseFadeOut, nullptr);
		cheese->runAction(cheeseSequence);
	}
	return true;
}
