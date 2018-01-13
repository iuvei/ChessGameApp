/************************************************************************************
 * file: 		GameDataMgr.h
 * copyright:	Cavan.Liu 2017
 * Author: 		Cavan.Liu
 * Create: 		2018/01/13 21:08:26
 * Description: 
 * Version	Author		Time			Description
 * V1.0    	Cavan.Liu	2018/01/13			
 *
 ************************************************************************************/

#ifndef __Game_Data_Mgr_hpp__
#define __Game_Data_Mgr_hpp__

#include <stdio.h>
#include "CocosHeader.h"
#include "../Pattern/Singleton.h"
#include "../Common/CMD_GameServer.h"
#include "../Common/GameDefine.h"
#include "../Common/struct.h"

//socket通信参数
struct tagSocketParam
{
	std::string m_sUrl;
	int m_nPort;

	tagSocketParam()
	{
		m_sUrl = "";
		m_nPort = 0;
	}
};

//发送网络数据参数
struct tagNetParam
{
	int m_nMainCode;
	int m_nSubCode;
	void *m_pData;
	int m_nDataLen;
	tagSocketParam m_socketParam;

	tagNetParam()
	{
		m_nMainCode = 0;
		m_nSubCode = 0;
		m_pData = nullptr;
		m_nDataLen = 0;
		m_socketParam = tagSocketParam();
	}
};

struct tagChatRecord
{
	DWORD           dwChatUserId;
	std::string     strChat;
	bool            bBrow;
	int             nIdx;
};

class GameDataMgr
{
	CREATE_SINGLETON_INIT(GameDataMgr, m_gameDataInstance, nullptr);
	void init();
public:
	void sendData(const tagNetParam &param);
	void sendCurrentRoomData(tagNetParam &param);
	void clear();

	CC_SYNTHESIZE(int, m_nRoomPort, RoomPort);
	CC_SYNTHESIZE(std::string, m_sRoomURL, RoomURL);

public:
	//游戏是否开始
	CC_SYNTHESIZE(bool, m_bIsGameStart, IsGameStart);
	//离开游戏
	CC_SYNTHESIZE(bool, m_bExit, IsExit);
	//是否是自己出牌
	CC_SYNTHESIZE(bool, m_bIsMyRound, MyRound);
	//自己是否是地主(牛牛中代表庄家)
	CC_SYNTHESIZE(bool, m_bIsLander, IsMineLander);
	//是否换桌
	CC_SYNTHESIZE(bool, m_bChangeDesk, IsChangeDesk);
	//一局结束
	CC_SYNTHESIZE(bool, m_bRoundOver, IsRoundOver);
	//登陆普通房间
	CC_SYNTHESIZE(bool, m_bEnterNormalRoom, EnterNormalRoom);
	//登陆防作弊房间
	CC_SYNTHESIZE(bool, m_bEnterAntiCheat, EnterAntiCheat);
	//防作弊房间进行分组
	CC_SYNTHESIZE(bool, m_bAntiCheatJoin, AntiCheatJoin);
	//选择坐下的桌椅
	CC_SYNTHESIZE(WORD, m_wSitTableID, SitTable);
	CC_SYNTHESIZE(WORD, m_wSitChairID, SitChair);
	//选择进入的房间
	CC_SYNTHESIZE(int, m_nEnterRoom, EnterRoom);
	//地主信息
	CC_SYNTHESIZE(tagBankerInfo, m_tagBankerInfo, BankerInfo);

	////////////////////////////////////////////////
	//MARK: 游戏聊天管理
public:
	void addRecord(const tagChatRecord &record);

	size_t recordCount(){ return m_vecChatRecord.size(); }

	bool getRecordAt(const size_t &idx, tagChatRecord &record);

	bool getStaticTextAt(const size_t &idx, tagChatRecord &record);

	bool compareWithStaticText(const std::string &src, size_t &idx);

	size_t staticRecordCount(){ return m_vecStaticChat.size(); }

	void clearRecord();
private:
	std::vector<tagChatRecord> m_vecChatRecord;
	std::vector<tagChatRecord> m_vecStaticChat;
	//MARK: 游戏聊天管理
public:
	/*
	* @brief 游戏场景，以本玩家为基准进行方向转换
	* @param[otherchair] 服务器传过来的玩家 chairid
	* @return 转换后的玩家方向
	*/
	enGameRoleDir getRoleDir(const WORD &otherchair);

	/*
	* @brief 根据自己的结算积分及自己是身份，确定地主、农民结算结果
	* @param[score] 自己的积分
	* @return enGameResult
	*/
	enGameResult getWinDir(const LONGLONG &llGameCoin);

	//方向是否有效
	bool isValidDir(const enGameRoleDir &dir);
#pragma mark-
#pragma mark 网络消息
public:
	//进入房间
	void connectRoom(const ST_GAME_ROOM_SERVER *pstGameRoomServer);
	void enterRoom(const std::string &roompass);
	//退出房间
	void leaveRoom(const WORD &wTabelID, const WORD &wChairID, const BYTE &bForceLeave);
	//配桌查询(除去自己之外的两个座位的信息)
	void queryChairInfo(const WORD &wTabelID, const WORD &myChair);
	//入桌坐下请求
	void requestSitdown(const WORD &wTabelID, const WORD &wChairID, const std::string &password = "");
	//随机入桌
	void randomSit();
	//准备请求
	void requestReady(bool bReady);
	//换桌请求
	void requestChangeDesk();
	//用户托管
	void userTrusteeship(const BYTE &flag);
	//获取场景
	void requestFrame();
	//分组确认
	void sureJoin();

	/*
	////////////////////////////////////////////////////////////////////////// 牛牛
	//用户叫庄
	void userCallBanker(const BYTE &btBanker);
	//用户加注
	void userAddScore(const LONGLONG &lAddScore);
	//用户摊牌
	void userOpenCard(const BYTE &bOpen);
	//////////////////////////////////////////////////////////////////////////
	*/

	void textChat(const int &len, const std::string &msg, const DWORD &tagetUser = INVALID_USERID);

	void browChat(const int &idx, const DWORD &tagetUser = INVALID_USERID);
#pragma mark-
private:
	static GameDataMgr* m_gameDataInstance;
	tagNetParam m_param;
};
#endif /* GameDataMgr_hpp */
