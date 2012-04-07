#ifndef NerdHerd_tracking_
#define NerdHerd_tracking_

#include "WPILib.h"
#include <vector>
#include "Logger.h"
#include "CSVReader.h"

class CameraTracking : public LogBase {
public:
	CameraTracking(Logger*, CSVReader*);
	~CameraTracking();
	std::string name();
	void log(FILE*);
	void reload();
	bool fresh;
	float distance;
	float horizantal;
	vector<ParticleAnalysisReport> targets;
	
	
private:
	static void s_TrackTask (CameraTracking *self);
	void TrackTask();
	CSVReader *config;
	Task TaskTrack;
	int _framecount;
	float heightToDistance(int height);
	static bool CompareParticles(ParticleAnalysisReport particle1, ParticleAnalysisReport particle2);
};

#endif // NerdHerd_tracking_
