#ifndef NerdHerd_tracking_
#define NerdHerd_tracking_

#include "WPILib.h"
#include <vector>
#include "Logger.h"
#include "CSVReader.h"

class CameraTracking : public LogBase {
public:
	struct TargetLocation{
		
	};
	CameraTracking(Logger*, CSVReader*);
	~CameraTracking();
	std::string name();
	void log(FILE*);
	bool fresh;
	float distance;
	float horizantial;
	vector<ParticleAnalysisReport> targets;
	
	
private:
	std::vector<TargetLocation> locations;
	static void s_TrackTask (CameraTracking *self);
	void TrackTask();
	Task TaskTrack;
	CSVReader *config;
	int _framecount;
	float heightToDistance(int height);
	static bool CompareParticles(ParticleAnalysisReport particle1, ParticleAnalysisReport particle2);
};

#endif // NerdHerd_tracking_
