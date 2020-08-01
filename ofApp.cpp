#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofColor color;
	vector<ofColor> color_list;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		color_list.push_back(color);
	}

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	for (int k = 0; k < 3; k++) {

		auto head_size = 8;

		ofMesh mesh;
		vector<glm::vec3> right, left, frame;
		auto noise_seed_deg = ofRandom(1000);
		auto noise_seed_radius = ofRandom(1000);

		glm::vec3 last_location;
		float last_theta;

		for (int i = 0; i < 25; i++) {

			auto noise_deg = ofMap(ofNoise(noise_seed_deg, (ofGetFrameNum() + i) * 0.001), 0, 1, -360, 360);
			auto noise_radius = ofMap(ofNoise(noise_seed_radius, (ofGetFrameNum() + i) * 0.01), 0, 1, -320, 320);
			auto next_noise_deg = ofMap(ofNoise(noise_seed_deg, (ofGetFrameNum() + i + 1) * 0.001), 0, 1, -360, 360);
			auto next_noise_radius = ofMap(ofNoise(noise_seed_radius, (ofGetFrameNum() + i + 1) * 0.01), 0, 1, -320, 320);

			auto location = glm::vec3(noise_radius * cos(noise_deg * DEG_TO_RAD), noise_radius * sin(noise_deg * DEG_TO_RAD), 0);
			auto next = glm::vec3(next_noise_radius * cos(next_noise_deg * DEG_TO_RAD), next_noise_radius * sin(next_noise_deg * DEG_TO_RAD), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, 25, 0, head_size) * cos(theta + PI * 0.5), ofMap(i, 0, 25, 0, head_size) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, 25, 0, head_size) * cos(theta - PI * 0.5), ofMap(i, 0, 25, 0, head_size) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
		}

		for (int i = 0; i < right.size(); i++) {

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);
		}

		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
		}

		mesh.addVertex(last_location);
		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(head_size * cos(theta), head_size * sin(theta), 0));
			frame.push_back(last_location + glm::vec3(head_size * cos(theta), head_size * sin(theta), 0));
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		for (int deg = 0; deg < 360; deg += 30) {

			ofRotate(30);

			ofSetColor(39);
			mesh.draw();
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}