#pragma once

#include "AT_Math.h"

namespace AT3
{ 

/** ��¼���������
*/
class Graphics2DCamera
{
public:
	/** ����������仯ʱ�Ļص����� */
	typedef void(*onCameraAdjust)(int sreenWidth, int screenHeight, const fVector2& lowPos, const fVector2& upperPos, void* param);

public:
	/** ���ô��ڴ�С */
	void setWindowSize(int width, int height);
	/** ��ô��ڴ�С */
	const iVector2& getWindowSize(void) const { return m_winSize; }

	/** ��Ļ����ת��Ϊ�߼����� */
	fVector2 convertScreenToWorld(int x, int y);

	/** ��Ļ����ת��Ϊ�߼����� */
	Real convertScreenSizeToWorld(Real size);

	/** ���÷���ϵ�� */
	void setViewZoom(Real viewZoom);
	/** ��÷���ϵ�� */
	Real getViewZoom(void) const { return m_viewZoom; }

	/** �ƶ������ */
	void pan(const fVector2& offset);

	/** ������Ҫ�۲�ĳ����Ĵ�С��
	* �ú�������ݳ����Ĵ�С����������������Ա�֤����������������Ұ�� 
	*/
	void setSceneSize(const fVector2& sceneSize);

protected:
	/** ���� */
	void _update(void);

protected:
	enum { DEFAULT_HALF_HEIGHT = 256, };	//!< ȱʡһ��߶���Ļ��Ӧ����ʵ�߶�

	iVector2 m_winSize;			//!< ���ڴ�С
	Real m_viewZoom;			//!< ����ϵ����ȱʡ1.0				
	fVector2 m_viewCenter;		//!< �ӵ����ģ�ȱʡ[0,0]

	onCameraAdjust m_callBack;	//!< �ص�����
	void* m_param;

public:
	Graphics2DCamera(onCameraAdjust callBack, void* param);
	~Graphics2DCamera();
};

}
