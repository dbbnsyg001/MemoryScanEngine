#pragma once
#include <QString>
enum EM_ScanType
{
    Unkown = 0,//δ֪��ֵ
    Exact,//��ȷ��ֵ
    GreaterThan,//��..�����ֵ
    LessThan,//��..С����ֵ
    Between,//��ֵλ��...֮��
    Increased,//���ӵ���ֵ
    Reduced,//���ٵ���ֵ
    Change,//�䶯����ֵ
    UnChange,//δ�䶯����ֵ
};
enum EM_ScanDataType
{
    FourBytes = 0,//4�ֽ�
    EightBytes,//8�ֽ�
    Float,//������
    Double,//˫����
};
/// <summary>
/// ����ʱ��UI����
/// </summary>
struct ST_ScanUIData
{
    bool isFirstScan = true; //�Ƿ��һ������
    bool isHexScan = false; //�����Ƿ�Ϊ16����
    EM_ScanType scanType;   //��������
    EM_ScanDataType scanDataType;//��������
    QString scanMemTarget = "All";
    int64_t scanMemStart = 0x0000000000000000;
    int64_t scanMemEnd = 0x7fffffffffffffff;
};