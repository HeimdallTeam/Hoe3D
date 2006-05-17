
#ifndef _HOE_BASE_SCENE_H_
#define _HOE_BASE_SCENE_H_

class HoeBaseScene : public IHoeScene
{
	// settings
	class XHoe2DCallback * m_2dcb;
public:
	HoeBaseScene();

	virtual void HOEAPI Set2DCallback(XHoe2DCallback *);
	virtual int HOEAPI RegisterObject(XHoeObject *, unsigned long flags = 0);
	/** @see IHoeScene::CreateParticleEmitor() */
	virtual IHoeParticleEmitor * HOEAPI CreateParticleEmitor();
	/** @see IHoeScene::CreateLight() */
	virtual IHoeLight * HOEAPI CreateLight(bool direct);

	virtual IHoeScenePhysics * HOEAPI GetScenePhysics();
	virtual IHoeSceneEnv * HOEAPI GetSceneEnv();
	virtual IHoeCamera * HOEAPI GetCamera();

	virtual void Process(const double dtime);
	virtual void Preprocess() {};
	virtual void Render();
	virtual void Paint2D();

	virtual void Delete();
	bool IsActive();
};

#endif // _HOE_BASE_SCENE_H_

