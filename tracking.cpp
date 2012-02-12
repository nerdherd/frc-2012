#include "tracking.h"

#include <iostream>
using namespace std;
#include <math.h>

CameraTracking::CameraTracking (Logger *log, CSVReader *con):
LogBase(log), config(con),
TaskTrack("Tracking", (FUNCPTR)s_TrackTask, 104, 100000) // large stack so that we can attempt to not allocate memory
{
	TaskTrack.Start((int)this);
	_framecount=0;
}
CameraTracking::~CameraTracking() {
	TaskTrack.Stop();
	AxisCamera::DeleteInstance();
}

void CameraTracking::s_TrackTask (CameraTracking *self) {
	//Wait(25); // give the camera some time to start up
	self->TrackTask();
}

float CameraTracking::heightToDistance (int height) {
	return 20105. * pow(height, -1.0592);
}

bool CameraTracking::CompareParticles(ParticleAnalysisReport par1, ParticleAnalysisReport par2) {
	//return particle1.particleToImagePercent > particle2.particleToImagePercent;
	return (par1.particleArea / (par1.boundingRect.height * par1.boundingRect.width)) > (par2.particleArea / (par2.boundingRect.height * par2.boundingRect.width));
}


void CameraTracking::TrackTask () {
	cout << "camera tracking task starting\n";
	AxisCamera &camera = AxisCamera::GetInstance("10.6.87.11");
	HSLImage image;
	//BinaryImage *convex = new BinaryImage;
	int error_count=0;
	while(1) {
		Wait(.1); // max 20 Hz
		if(error_count > 50) {
			// resting camera
			/*AxisCamera::DeleteInstance();
			Wait(1);
			camera = &AxisCamera::GetInstance("10.6.87.11");
			Wait(.5);
			*/
			camera.RestartCameraTask();
			Wait(2);
			
			error_count=0;
		}
		if(0 == camera.GetImage(&image)) {
			printf("did not get an image\n");
			Wait(.1);
			error_count+=2;
			continue;
		}
		if(!camera.IsFreshImage()) {
			// idk what is happening here
			//printf("did not get a fresh image\n");
			Wait(.1);
			error_count++;
			continue;
		}
		image.Write("income_img.jpg");
		BinaryImage *Threshold = // this needs to get deleted by the end
		image.ThresholdHSL((int)config->GetValue("imageHLow"), (int)config->GetValue("imageHHigh"),
						   (int)config->GetValue("imageSLow"), (int)config->GetValue("imageSHigh"),
						   (int)config->GetValue("imageLLow"), (int)config->GetValue("imageLHigh"));
		
		//image.ThresholdHSL(0,25500,0,25500,0,25500);
		//image.ThresholdRGB(110,130,125,145,80,100);
		/*MonoImage *mm = image.GetIntensityPlane();
		mm->Write("intensity.png");
		delete mm;
		mm = image.GetHSLHuePlane ();
		mm->Write("hue.png");
		delete mm;*/
		//mm = image.GetHSLSaturationPlane ();
		//mm->Write("sat.jpg");
		//delete mm;
		
		Threshold->Write("threshold_img.png");
		
		// this feels really bad
		// but there are no new variables and the constructor is empty so there should be no problems
		BinaryImage *bThreshold = static_cast<BinaryImage*>(Threshold);
		
		BinaryImage *convex = bThreshold->ConvexHull(true);
		
		convex->Write("convex_img.png");
		
		//vector<ParticleAnalysisReport>* CAMresults = convex->GetOrderedParticleAnalysisReports();
		//vector<ParticleAnalysisReport> targets;
		double thresholdValue = config->GetValue("imageTargetThreshold");
		/*
		for(vector<ParticleAnalysisReport>::iterator it=CAMresults->begin(), end=CAMresults->end();it!=end;it++) {
			
			printf("particle %f %f %f %f %f\n", it->particleToImagePercent, it->particleArea / (it->boundingRect.height * it->boundingRect.width), it->boundingRect.height * it->boundingRect.width, it->particleArea, it->particleQuality);
			targets.push_back(*it);
			break;
		}*/
		
		vector<ParticleAnalysisReport> particles;
		int particleCount = convex->GetNumberParticles();
		for(int particleIndex = 0; particleIndex < particleCount; particleIndex++)
		{
			ParticleAnalysisReport par = convex->GetParticleAnalysisReport(particleIndex);
			//printf("area %f\n", par.particleArea / (par.boundingRect.height * par.boundingRect.width));
			if((par.particleArea / (par.boundingRect.height * par.boundingRect.width)) > thresholdValue 
					&& par.boundingRect.width > 25 // anything smaller than this will never be the target
					&& par.boundingRect.height > 25)
				particles.push_back(par);
		}
		//sort(particles.begin(), particles.end(), CompareParticles);
		sort(particles.begin(), particles.end(), CompareParticles);
		
		
		/*for(vector<ParticleAnalysisReport>::iterator it = particles.begin();it!=particles.end();it++) {
			printf("distance %i %f %f %f\n", it->boundingRect.height, heightToDistance(it->boundingRect.height), it->center_mass_x_normalized, it->center_mass_y_normalized);
		}*/
		
		if(particles.size() > 0) {
			distance = heightToDistance(particles[0].boundingRect.height);
			horizantial = particles[0].center_mass_x_normalized;
			fresh = true;
		}else{
			fresh = false;
		}
		
		_framecount++;
		//printf("camera is good\n");
		error_count=0;
		Wait(.1); // change this to be a lot smaller, it not 0
		// clean up
		delete Threshold;
		delete convex;
		//delete CAMresults;
	}
}

std::string CameraTracking::name() {
	return "camera frames";
}

void CameraTracking::log(FILE *f) {
	fprintf(f,",%i",_framecount);
}

