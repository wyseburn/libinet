
#include "os/include/WordFilter.h"

using namespace IGame;

int main()
{
    WordFilter filter;
    std::vector<std::string> tmpVector;
    tmpVector.push_back("fuck");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("��b");
    tmpVector.push_back("��B");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("��Ů");
    tmpVector.push_back("��ˮ��");
    tmpVector.push_back("��ί");
    tmpVector.push_back("潼�");
    tmpVector.push_back("潸�");
    tmpVector.push_back("����");
    tmpVector.push_back("�ٱ�");
    tmpVector.push_back("�ض�");
    tmpVector.push_back("�ض�");
    tmpVector.push_back("��ʲ");
    tmpVector.push_back("����");
    tmpVector.push_back("���");
    tmpVector.push_back("������");
    tmpVector.push_back("������");
    tmpVector.push_back("��Ƥ");
    tmpVector.push_back("�˾�ѧ��");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("Gm");
    tmpVector.push_back("GM");
    tmpVector.push_back("���ǲ���");
    tmpVector.push_back("��ͻ��");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("����̨");
    tmpVector.push_back("���㵺");
    tmpVector.push_back("�۹�");
    tmpVector.push_back("��Сƽ");
    tmpVector.push_back("����");
    tmpVector.push_back("��Ѫ��");
    tmpVector.push_back("��Ѫb");
    tmpVector.push_back("��ѪB");
    tmpVector.push_back("��ͳ");
    tmpVector.push_back("������");
    tmpVector.push_back("���ձ��۹�");
    tmpVector.push_back("���Ԫ");
    tmpVector.push_back("�󷨵���");
    tmpVector.push_back("��");
    tmpVector.push_back("������");
    tmpVector.push_back("��Ͷ");
    tmpVector.push_back("����Ա");
    tmpVector.push_back("�߳�");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("��b");
    tmpVector.push_back("��B");
    tmpVector.push_back("����");
    tmpVector.push_back("�ؽ�");
    tmpVector.push_back("����");
    tmpVector.push_back("�ɱ�");
    tmpVector.push_back("�ɱ�");
    tmpVector.push_back("��b");
    tmpVector.push_back("��B");
    tmpVector.push_back("������");
    tmpVector.push_back("����m");
    tmpVector.push_back("������");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("���ֹ�");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("��Ʒ");
    tmpVector.push_back("����");
    tmpVector.push_back("����ϯ");
    tmpVector.push_back("������");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("��b");
    tmpVector.push_back("��B");
    tmpVector.push_back("������");
    tmpVector.push_back("��Ů");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("��Ƭ");
    tmpVector.push_back("�ƻݽ�");
    tmpVector.push_back("�ƾ�");
    tmpVector.push_back("�ʾ�");
    tmpVector.push_back("����ϯ");
    tmpVector.push_back("������");
    tmpVector.push_back("�����");
    tmpVector.push_back("����");
    tmpVector.push_back("������");
    tmpVector.push_back("����");
    tmpVector.push_back("����Ժ");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("��ͷ");
    tmpVector.push_back("�ٷ�");
    tmpVector.push_back("����");
    tmpVector.push_back("��ƨ");
    tmpVector.push_back("����");
    tmpVector.push_back("������");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("��������");
    tmpVector.push_back("����");
    tmpVector.push_back("��");
    tmpVector.push_back("�޸�");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("������");
    tmpVector.push_back("�ֿ�");
    tmpVector.push_back("��Ρ");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("�����");
    tmpVector.push_back("���־");
    tmpVector.push_back("���־");
    tmpVector.push_back("���");
    tmpVector.push_back("����");
    tmpVector.push_back("�ڽ�");
    tmpVector.push_back("�ڱ�");
    tmpVector.push_back("�ֲ�����");
    tmpVector.push_back("�ͷ�GM");
    tmpVector.push_back("�ͷ�");
    tmpVector.push_back("����");
    tmpVector.push_back("��Һ");
    tmpVector.push_back("��͢ѫ");
    tmpVector.push_back("�д�");
    tmpVector.push_back("����ʯ");
    tmpVector.push_back("����");
    tmpVector.push_back("������");
    tmpVector.push_back("����");
    tmpVector.push_back("ɽ����");
    tmpVector.push_back("��������");
    tmpVector.push_back("��K��");
    tmpVector.push_back("����ķ");
    tmpVector.push_back("�վ�");
    tmpVector.push_back("�ձ�");
    tmpVector.push_back("�ձ�");
    tmpVector.push_back("�����");
    tmpVector.push_back("�˴�");
    tmpVector.push_back("ǿ��");
    tmpVector.push_back("ƨ��");
    tmpVector.push_back("���");
    tmpVector.push_back("����");
    tmpVector.push_back("��ĸ");
    tmpVector.push_back("����ĸ");
    tmpVector.push_back("�ɴ�");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("÷��");
    tmpVector.push_back("ë��ϯ");
    tmpVector.push_back("ë��");
    tmpVector.push_back("ëƬ");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("���༧");
    tmpVector.push_back("��ҫ");
    tmpVector.push_back("����");
    tmpVector.push_back("����ǿ");
    tmpVector.push_back("������");
    tmpVector.push_back("�Խ�");
    tmpVector.push_back("�½�����");
    tmpVector.push_back("а��");
    tmpVector.push_back("СȪ��һ��");
    tmpVector.push_back("С����");
    tmpVector.push_back("ϵͳ����");
    tmpVector.push_back("���ض���");
    tmpVector.push_back("����");
    tmpVector.push_back("�����");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("�ĸ�");
    tmpVector.push_back("�¼ұ�");
    tmpVector.push_back("������");
    tmpVector.push_back("���");
    tmpVector.push_back("���˵�");
    tmpVector.push_back("����");
    tmpVector.push_back("�찲���¼�");
    tmpVector.push_back("̫�ӵ�");
    tmpVector.push_back("̨�����");
    tmpVector.push_back("̨��");
    tmpVector.push_back("̨��");
    tmpVector.push_back("̨��");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("�߹�");
    tmpVector.push_back("�Է�");
    tmpVector.push_back("��ϯ");
    tmpVector.push_back("���F��");
    tmpVector.push_back("���");
    tmpVector.push_back("������");
    tmpVector.push_back("�ܶ���");
    tmpVector.push_back("���Ϻ�");
    tmpVector.push_back("�л����񹲺͹�");
    tmpVector.push_back("�л����");
    tmpVector.push_back("�й�");
    tmpVector.push_back("�й�����");
    tmpVector.push_back("�й�");
    tmpVector.push_back("֧��");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("�䱦��");
    tmpVector.push_back("�����");
    tmpVector.push_back("�췴");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("��Ӫ");
    tmpVector.push_back("�¾�");
    tmpVector.push_back("��Ϸ����Ա");
    tmpVector.push_back("��������");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("����");
    tmpVector.push_back("������");
    tmpVector.push_back("������");
    tmpVector.push_back("ҡͷ��");
    tmpVector.push_back("��ҫ");
    tmpVector.push_back("��");
    tmpVector.push_back("��Ϸ");

    
    std::vector<std::string> tmpVector1;
    filter.AddWords(tmpVector);

    char buf[1234] = "�ٰ��ٰ����ٰ��򷢵ط�";

    filter.AddWord("��");

    std::string xx = "aFu��Ckb�����ads�ձ�af�ٱ�adkjf���������Ϸ����Ա�򷢷���Ϸ��#��##��";

    printf("src:%s\n", xx.c_str());

    int x = filter.Check(xx.c_str());

    filter.CheckAndModify(xx);

    printf("dst:%s\n", xx.c_str());
    return 0;
}


