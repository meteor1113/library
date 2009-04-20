//


#include "stdafx.h"
#include "dumpxml.h"

#include <string>
#include <windows.h>

#include "encode.h"

using namespace dumpxml;

template<class T>
struct BinaryTreeNode : public TiXmlBindCustom
{
    string m_Attribute;
    BinaryTreeNode *m_LeftChild;
    BinaryTreeNode *m_RightChild;
    T m_Value;

    BinaryTreeNode(const T &Value = T(), const string & Attribute = "Normal")
        : m_Value(Value)
        , m_Attribute(Attribute)
        , m_LeftChild(NULL)
        , m_RightChild(NULL)
        {
        }

    ~BinaryTreeNode()
        {
            delete m_LeftChild;
            delete m_RightChild;
        }

    void assign( const BinaryTreeNode *pNode )
        {
            m_Attribute = pNode->m_Attribute;
            m_Value = pNode->m_Value;
            m_LeftChild = pNode->m_LeftChild?pNode->m_LeftChild->Clone():NULL;
            m_RightChild =
                pNode->m_RightChild?pNode->m_RightChild->Clone():NULL;
        }

    BinaryTreeNode( const BinaryTreeNode& spNode )
        {
            assign(&spNode);
        }

    BinaryTreeNode & operator=( const BinaryTreeNode& spNode )
        {
            if (this != &spNode)
            {
                delete m_LeftChild;
                delete m_RightChild;
                assign(&spNode);
            }
            return *this;
        }

    BinaryTreeNode * Clone()
        {
            BinaryTreeNode *p = new BinaryTreeNode;
            p->assign(this);
            return p;
        }

    static BinaryTreeNode TiXmlGetValue(const TiXmlNode * Node)
        {
            BinaryTreeNode s;
            s.m_Attribute = Node->ToElement()->Attribute("Attribute");
            TiXmlChildToVar(s.m_Value, Node, "Value");
            const TiXmlNode * pSpNode = Node->FirstChild("LeftChild");
            if (pSpNode) s.m_LeftChild = TiXmlGetValue(pSpNode).Clone();
            pSpNode = Node->FirstChild("RightChild");
            if (pSpNode) s.m_RightChild = TiXmlGetValue(pSpNode).Clone();
            return s;
        }

    static TiXmlElement* GetTiXml( const BinaryTreeNode &val,
                                   const string & Name)
        {
            TiXmlElement * Node = new TiXmlElement(Name.c_str());
            Node->SetAttribute("Attribute", val.m_Attribute.c_str());
            Node->LinkEndChild(VarToTiXml(val.m_Value, "Value"));
            if (val.m_LeftChild)
                VarToTiXmlChild(*val.m_LeftChild, Node, "LeftChild");
            if (val.m_RightChild)
                VarToTiXmlChild(*val.m_RightChild, Node, "RightChild");
            return Node;
        }
};

// Config Singleton£¬
class CConfig : public TiXmlTieSaveLoad
{
public:
    struct CProduct : public TiXmlBindCustom
    {
        string Code;
        string ChineseName;
        double MultiRatio;
        double MinChange;
        string Exchange;

        static TiXmlElement *GetTiXml(const CProduct &s,
                                      const string & Name = "item")
            {
                TiXmlElement * Node = new TiXmlElement(Name.c_str());
                VarToTiXmlChild(s.ChineseName   , Node, "ChineseName");
                VarToTiXmlChild(s.MultiRatio    , Node, "MultiRatio");
                VarToTiXmlChild(s.MinChange             , Node, "MinChange");
                VarToTiXmlChild(s.Exchange              , Node, "Exchange");
                return Node;
            }

        static CProduct TiXmlGetValue(const TiXmlNode * Node)
            {
                CProduct s;
                TiXmlChildToVar(s.ChineseName   , Node, "ChineseName");
                TiXmlChildToVar(s.MultiRatio    , Node, "MultiRatio");
                TiXmlChildToVar(s.MinChange             , Node, "MinChange");
                TiXmlChildToVar(s.Exchange              , Node, "Exchange");
                return s;
            }
    };

    bool m_bLoginSaveUser;
    string m_LoginUserID;
    vector<string> m_TestVS;
    map<string, CProduct> m_Products;
    map<string, map<string, string> > m_Skin;

protected:

    CConfig(void)
        {
            AddTiXmlTie(m_bLoginSaveUser, "LoginSaveUser");
            AddTiXmlTie(m_LoginUserID, "LoginUserID");
            AddTiXmlTie(m_TestVS, "vector_string");
            AddTiXmlTie(m_Products, "Products");
            AddTiXmlTie(m_Skin, "Skin");

            LoadFile("config.xml");
        }

public:
    ~CConfig(void)
        {
            SaveFile("config.xml");
        }

    static CConfig& CConfig::Instance()
        {
            static CConfig _instance;
            return _instance;
        }
};


class SLanServerConfig : public TiXmlTieSaveLoad
{
public:
    struct DB : public TiXmlBindCustom
    {
        int DBType;
        wstring ServerName;
        wstring DatabaseName;
        wstring UserID;
        wstring Password;

        static TiXmlElement *GetTiXml(const DB &s, const string & name = "item")
            {
                TiXmlElement * node = new TiXmlElement(name.c_str());
                VarToTiXmlChild(s.DBType, node, "DBType");
                VarToTiXmlChild(utility::encode::ConvertToUtf8(s.ServerName),
                                node, "ServerName");
                VarToTiXmlChild(utility::encode::ConvertToUtf8(s.DatabaseName),
                                node, "DatabaseName");
                VarToTiXmlChild(utility::encode::ConvertToUtf8(s.UserID),
                                node, "UserID");
                VarToTiXmlChild(utility::encode::ConvertToUtf8(s.Password),
                                node, "Password");

//            std::string str = ConvertAsciiToUtf8(s.DatabaseName);
                return node;
            }

        static DB TiXmlGetValue(const TiXmlNode * node)
            {
                DB s;
                std::string utf8Str;
                TiXmlChildToVar(s.DBType, node, "DBType");
                TiXmlChildToVar(utf8Str, node, "ServerName");
                s.ServerName = utility::encode::ConvertUtf8ToWstring(utf8Str);
                TiXmlChildToVar(utf8Str, node, "DatabaseName");
                s.DatabaseName = utility::encode::ConvertUtf8ToWstring(utf8Str);
                TiXmlChildToVar(utf8Str, node, "UserID");
                s.UserID = utility::encode::ConvertUtf8ToWstring(utf8Str);
                TiXmlChildToVar(utf8Str, node, "Password");
                s.Password = utility::encode::ConvertUtf8ToWstring(utf8Str);

                //std::string str = ConvertUtf8ToAscii(s.DatabaseName);
                return s;
            }
    };

    struct Alarm : public TiXmlBindCustom
    {
        int Level1;
        int Level2;
        int Level3;
        int Level4;
        int Level5;

        static TiXmlElement *GetTiXml(const Alarm &s,
                                      const string & name = "item")
            {
                TiXmlElement * node = new TiXmlElement(name.c_str());
                VarToTiXmlChild(s.Level1, node, "Level1");
                VarToTiXmlChild(s.Level2, node, "Level2");
                VarToTiXmlChild(s.Level3, node, "Level3");
                VarToTiXmlChild(s.Level4, node, "Level4");
                VarToTiXmlChild(s.Level5, node, "Level5");
                return node;
            }

        static Alarm TiXmlGetValue(const TiXmlNode * node)
            {
                Alarm s;
                TiXmlChildToVar(s.Level1, node, "Level1");
                TiXmlChildToVar(s.Level2, node, "Level2");
                TiXmlChildToVar(s.Level3, node, "Level3");
                TiXmlChildToVar(s.Level4, node, "Level4");
                TiXmlChildToVar(s.Level5, node, "Level5");
                return s;
            }
    };

    struct EmailAlarm : public TiXmlBindCustom
    {
        wstring SMTPServerIP;
        int SMTPServerPort;
        int SendAuth;
        wstring UserName;
        wstring Password;

        static TiXmlElement *GetTiXml(const EmailAlarm &s,
                                      const string & name = "item")
            {
                TiXmlElement * node = new TiXmlElement(name.c_str());
                VarToTiXmlChild(utility::encode::ConvertToUtf8(s.SMTPServerIP),
                                node, "SMTPServerIP");
                VarToTiXmlChild(s.SMTPServerPort, node, "SMTPServerPort");
                VarToTiXmlChild(s.SendAuth, node, "SendAuth");
                VarToTiXmlChild(utility::encode::ConvertToUtf8(s.UserName),
                                node, "UserName");
                VarToTiXmlChild(utility::encode::ConvertToUtf8(s.Password),
                                node, "Password");
                return node;
            }

        static EmailAlarm TiXmlGetValue(const TiXmlNode * node)
            {
                EmailAlarm s;
                std::string utf8Str;
                TiXmlChildToVar(utf8Str, node, "SMTPServerIP");
                s.SMTPServerIP = utility::encode::ConvertUtf8ToWstring(utf8Str);
                TiXmlChildToVar(s.SMTPServerPort, node, "SMTPServerPort");
                TiXmlChildToVar(s.SendAuth, node, "SendAuth");
                TiXmlChildToVar(utf8Str, node, "UserName");
                s.UserName = utility::encode::ConvertUtf8ToWstring(utf8Str);
                TiXmlChildToVar(utf8Str, node, "Password");
                s.Password = utility::encode::ConvertUtf8ToWstring(utf8Str);

                return s;
            }
    };

    DB mDb;
    Alarm mAlarm;
    EmailAlarm mEmailAlarm;

protected:

    SLanServerConfig(void)
        {
            AddTiXmlTie(mDb, "DB");
            AddTiXmlTie(mAlarm, "Alarm");
            AddTiXmlTie(mEmailAlarm, "EmailAlarm");

            LoadFile("SLanServer.cfg");
        }

public:
    ~SLanServerConfig(void)
        {
            SaveFile("SLanServer.cfg", "UTF-8");
        }

    static SLanServerConfig& SLanServerConfig::Instance()
        {
            static SLanServerConfig sInstance;
            return sInstance;
        }
};



int _tmain(int argc, _TCHAR* argv[])
{
    list<map<int, vector<string> > > map_list;

    map<int, vector<string> > ivs_map;
    ivs_map[20].push_back("hello");
    ivs_map[20].push_back("world");
    map_list.push_back(ivs_map);
    ivs_map[20][0] = "haha";
    ivs_map[99].push_back("what?");
    map_list.push_back(ivs_map);
    ivs_map[10].push_back("oneaddone");
    ivs_map[99].push_back("newsmth");
    map_list.push_back(ivs_map);

    //
    VarToTiXmlFile(map_list, "map_list.xml");


    vector<vector<int> > ivv;
    //
    TiXmlHelper::TiXmlFileToVar(ivv, "ivv.xml");

    for(vector<vector<int> >::iterator it = ivv.begin(); it != ivv.end(); ++it)
    {
        for(vector<int>::iterator itv = it->begin(); itv != it->end(); ++itv)
        {
            (*itv)++;
        }
        it->push_back(0);
    }
    ivv.push_back(vector<int>());

    //
    TiXmlHelper::VarToTiXmlFile(ivv, "ivv.xml");


    //
    BinaryTreeNode<int> Tree(10);
    Tree.m_LeftChild = new BinaryTreeNode<int>(20);
    Tree.m_RightChild = new BinaryTreeNode<int>(30);
    Tree.m_RightChild->m_LeftChild = new BinaryTreeNode<int>(40);

    TiXmlHelper::VarToTiXmlFile(Tree, "Tree.xml");

    CConfig::Instance().m_LoginUserID = "hello";
    CConfig::Instance().m_bLoginSaveUser =
        !CConfig::Instance().m_bLoginSaveUser;

    wstring a = SLanServerConfig::Instance().mDb.DatabaseName;
    SLanServerConfig::Instance().mDb.DBType = 1;
    SLanServerConfig::Instance().mDb.ServerName = L"ddd";
    SLanServerConfig::Instance().mDb.DatabaseName = L"just english";
    SLanServerConfig::Instance().mDb.UserID = L"a";
    SLanServerConfig::Instance().mDb.Password = L"a";

    SLanServerConfig::Instance().mAlarm.Level1 = 1;
    SLanServerConfig::Instance().mAlarm.Level2 = 1;
    SLanServerConfig::Instance().mAlarm.Level3 = 0;
    SLanServerConfig::Instance().mAlarm.Level4 = 1;
    SLanServerConfig::Instance().mAlarm.Level5 = 1;

    SLanServerConfig::Instance().mEmailAlarm.SMTPServerIP = L"192.168.1.19";
    SLanServerConfig::Instance().mEmailAlarm.SMTPServerPort = 25;
    SLanServerConfig::Instance().mEmailAlarm.SendAuth = 1;
    SLanServerConfig::Instance().mEmailAlarm.UserName =
        L"lxin.informedia.net.cn";
    SLanServerConfig::Instance().mEmailAlarm.Password = L"123456";


    return 0;
}
