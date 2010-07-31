
#include "os/include/Container.h"

using namespace IGame;

void DemoList()
{
    IGame::List<int> l;

    IGame::List<int>::Node* node1 = l.InsertHead(1);
    IGame::List<int>::Node* node2 = l.InsertHead(2);

    IGame::List<int>::Node* node3 = l.InsertTail(4);
    IGame::List<int>::Node* node4 = l.InsertTail(3);

    IGame::List<int>::Node* node = l.AllocNode(100);
    assert(node);
    l.InsertBefore(node2, node);
    l.Remove(node3);
    l.InsertAfter(node4, node3);

    IGame::List<int>::Node* n = l.GetN(2);
    printf("%d\n", n->m_Value);

    for (IGame::List<int>::Node* node = l.Begin();
        node != l.End(); node = l.Next(node))
    {
        printf("%d\n", node->m_Value);
    }
}

void DemoMap()
{
    IGame::Map<int, std::string> m;

    m.Insert(100, "aaaa");
    m.Insert(200, "bbbb");
    m.Insert(400, "cccc");

    printf("%s\n", m[100].c_str());
    printf("%s\n", m[200].c_str());

    printf("size: %d\n", m.Size());

    m.Erase(100);

    IGame::Map<int, std::string>::Node* node;

    for (node = m.Begin(); node != m.End(); node = m.Next(node))
    {
        printf("%d, %s\n", node->m_Key, node->m_Value.c_str());
    }

    node = m.Find(200);
    if (node)
    {
        printf("%s\n", node->m_Value.c_str());
    }
}

void DemoSet()
{
    IGame::Set<std::string> s;

    s.Insert("aaaa");
    s.Insert("bbbb");
    s.Insert("cccc");

    printf("%s\n", s["aaaa"].c_str());
    printf("%s\n", s["bbbb"].c_str());
    printf("%s\n", s["cccc"].c_str());

    printf("size: %d\n", s.Size());

    s.Erase("aaaa");

    IGame::Set<std::string>::Node* node;

    for (node = s.Begin(); node != s.End(); node = s.Next(node))
    {
        printf("%s\n", node->m_Value.c_str());
    }

    node = s.Find("cccc");
    if (node)
    {
        printf("%s\n", node->m_Value.c_str());
    }
}

void DemoPointerArray()
{
    IGame::PointerArray<void*> a;

    _UInt32 i[3] = { 0, 1, 2};

    _UInt32 index = a.Insert(&i[0]);
    a.Insert(index+1, &i[1]);
    a.Insert(index+2, &i[2]);

    printf("%d\n", *(_UInt32*)a[index]);
    printf("%d\n", *(_UInt32*)a[index+1]);
    printf("%d\n", *(_UInt32*)a[index+2]);

    a.Erase(index+1);

    _UInt32 n = *(_UInt32*)a.Find(index+2);
    printf("%d\n", n);
}

void DemoTireTree()
{
    TireTree t;
    t.AddWord("abcd", 1);
    t.AddWord("cdadf", 2);
    t.AddWord("fuck", 3);
    t.AddWord("大东亚", 4);
    t.AddWord("戳比", 5);
    t.AddWord("戳逼", 6);
    t.AddWord("戳b", 7);
    t.AddWord("戳B", 8);
    t.AddWord("吹箫", 9);
    t.AddWord("传销", 10);
    t.AddWord("处女", 11);
    t.AddWord("陈水扁", 12);
    t.AddWord("常委", 14);
    t.AddWord("娼妓", 15);
    t.AddWord("娼妇", 16);
    t.AddWord("插你", 17);
    t.AddWord("操逼", 18);
    t.AddWord("藏独", 19);
    t.AddWord("藏毒", 20);
    t.AddWord("布什", 21);
    t.AddWord("操", 22);
    t.AddWord("婊子", 23);
    t.AddWord("避孕套", 24);
    t.AddWord("本拉登", 25);
    t.AddWord("包皮", 26);
    t.AddWord("八九学潮", 27);
    t.AddWord("安南", 28);
    t.AddWord("爱兹", 29);
    t.AddWord("Gm", 30);
    t.AddWord("GM", 31);
    t.AddWord("东亚病夫", 32);
    t.AddWord("东突厥", 33);
    t.AddWord("东京", 34);
    t.AddWord("腚眼", 35);
    t.AddWord("钓鱼台", 36);
    t.AddWord("钓鱼岛", 37);
    t.AddWord("帝国", 38);
    t.AddWord("邓小平", 39);
    t.AddWord("荡妇", 40);
    t.AddWord("大血逼", 41);
    t.AddWord("大血b", 42);
    t.AddWord("大血B", 43);
    t.AddWord("大统", 44);
    t.AddWord("大世纪", 45);
    t.AddWord("大日本帝国", 46);
    t.AddWord("大纪元", 47);
    t.AddWord("大法弟子", 48);
    t.AddWord("大法", 49);
    t.AddWord("共产党", 50);
    t.AddWord("公投", 51);
    t.AddWord("管理员", 52);
    t.AddWord("高潮", 53);
    t.AddWord("戆比", 54);
    t.AddWord("戆逼", 55);
    t.AddWord("戆b", 56);
    t.AddWord("戆B", 57);
    t.AddWord("肛门", 58);
    t.AddWord("肛交", 59);
    t.AddWord("干你", 60);
    t.AddWord("干比", 61);
    t.AddWord("干逼", 62);
    t.AddWord("干b", 63);
    t.AddWord("干B", 64);
    t.AddWord("反人民", 65);
    t.AddWord("反共m", 66);
    t.AddWord("反革命", 67);
    t.AddWord("反动", 68);
    t.AddWord("反党", 69);
    t.AddWord("法轮功", 70);
    t.AddWord("法轮", 71);
    t.AddWord("独立", 72);
    t.AddWord("独裁", 73);
    t.AddWord("毒品", 74);
    t.AddWord("疆毒", 75);
    t.AddWord("江主席", 76);
    t.AddWord("江泽民", 77);
    t.AddWord("贱种", 78);
    t.AddWord("贱逼", 79);
    t.AddWord("贱b", 80);
    t.AddWord("贱B", 81);
    t.AddWord("贾庆林", 82);
    t.AddWord("妓女", 83);
    t.AddWord("鸡吧", 84);
    t.AddWord("机八", 85);
    t.AddWord("黄片", 86);
    t.AddWord("黄惠江", 87);
    t.AddWord("黄菊", 88);
    t.AddWord("皇军", 89);
    t.AddWord("胡主席", 90);
    t.AddWord("胡锦涛", 91);
    t.AddWord("黑社会", 92);
    t.AddWord("黑人", 93);
    t.AddWord("核武器", 94);
    t.AddWord("汉奸", 95);
    t.AddWord("国务院", 96);
    t.AddWord("国民党", 97);
    t.AddWord("鬼子", 98);
    t.AddWord("龟头", 99);
    t.AddWord("官方", 100);

    _UInt32 key1 = t.GetWord("江泽民");
    _UInt32 key2 = t.GetWord("汉奸");
    _UInt32 key3 = t.GetWord("操");
    _UInt32 key4 = t.GetWord("操逼");
    _UInt32 key5 = t.GetWord("大日本帝国");
    _UInt32 key6 = t.GetWord("cdadf");
    _UInt32 key7 = t.GetWord("贱B");

    printf("%d\n", key1);
}

int main()
{
    DemoList();
    DemoMap();
    DemoSet();
    DemoPointerArray();
    DemoTireTree();
    return 0;
}



