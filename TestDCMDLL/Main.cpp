#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif 

#include "dcmdll/dcmimage/dcmimage.h"
#include "dcmdll/dcmfile/dcmfile.h"
#include "dcmtk/dcmdata/dcddirif.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmpstat/dvpstat.h"
#include "dcmtk/dcmpstat/dvpsgr.h"  
#include "dcmtk/dcmpstat/dvpstx.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Main.h"

#undef main

using namespace std;

void load(int argc, char const* argv[]) {
	if (argc == 1) {
		cout << "Enter '-h' to view help\n";
		cout << "Exit.\n";
		exit(1);
	}
	else if (argc > 1) {
		if (strcmp(argv[1], "-h") == 0) {
			cout << "Command format: -i 'input_filename' -o 'output_filename' ...\n";

			cout << "For rotating image:\n";
			cout << "\t+Rl\t--rotate-left\n";
			cout << "\t+Rr\t--rotate-right\n";
			cout << "\t+Rtd\t--rotate-top-down\n";

			cout << "For flipping image:\n";
			cout << "\t+Fh\t--flip-horizontally\n";
			cout << "\t+Fv\t--flip-vertically\n";
			cout << "\t+Fba\t--flip-both-axes\n";

			cout << "For scaling image by factor\n";
			cout << "\t+Sxf 'factor'(float)\t--scale-x-by-factor\n";
			cout << "\t+Syf 'factor'(float)\t--scale-y-by-factor\n";

			cout << "For clipping and scaling image by factor\n";
			cout << "\t+Cxf 'left'(int) 'top'(int) 'width'(int) 'height'(int) 'factor'(float)\t--clip-and-scale-x-by-factor\n";
			cout << "\t+Cyf 'left'(int) 'top'(int) 'width'(int) 'height'(int) 'factor'(float)\t--clip-and-scale-y-by-factor\n";

			cout << "For changing contrast of image:\n";
			cout << "\t+Cc\t--change-contrast\n";

			cout << "For applying VOI LUT transformation:\n";
			cout << "\t+Wfl (default) --set-VOI-LUT-function-to-linear\n";
			cout << "\t+Wfs --set-VOI-LUT-function-to-sigmoid\n";
			cout << "\t+Wi 'n'(signed int) --apply-n-th-existing-VOI-windows\n";
			cout << "\t+Wm --apply-min-max-VOI-windows\n";
			cout << "\t+Wh 'n'(signed int) --apply-histogram-VOI-windows-ignore-n-percent\n";
			cout << "\t+Ww 'center'(float) 'width'(float) --apply-setting-VOI-windows\n";
		}
		else if (strcmp(argv[1], "-i") == 0 && argc >= 5) {
			char* inputFilename = new char[strlen(argv[2]) + 1];
			strcpy(inputFilename, argv[2]);

			if (strcmp(argv[3], "-o") == 0) {
				char* outputFilename = new char[strlen(argv[4]) + 1];
				strcpy(outputFilename, argv[4]);

				VOI_LUT_FUNCTION voiLutFunc = LINEAR;

				for (int i = 5; i < argc; ++i) {
					char* command = new char[strlen(argv[i]) + 1];
					strcpy(command, argv[i]);

					if (strcmp(command, "+Rl") == 0) {
						if (rotateImage(inputFilename, outputFilename, ROTATE_LEFT)) {
							cout << "Rotate image left successfully\n";
						}
						else {
							cout << "Failed to rotate image left\n";
						}
					}
					else if (strcmp(command, "+Rr") == 0) {
						if (rotateImage(inputFilename, outputFilename, ROTATE_RIGHT)) {
							cout << "Rotate image right successfully\n";
						}
						else {
							cout << "Failed to rotate image right\n";
						}
					}
					else if (strcmp(command, "+Rtd") == 0) {
						if (rotateImage(inputFilename, outputFilename, ROTATE_TOP_DOWN)) {
							cout << "Rotate image top-down successfully\n";
						}
						else {
							cout << "Failed to rotate image top-down\n";
						}
					}
					else if (strcmp(command, "+Fh") == 0) {
						if (flipImage(inputFilename, outputFilename, FLIP_HORIZONTALLY)) {
							cout << "Flip image horizontally successfully\n";
						}
						else {
							cout << "Failed to flip image horizontally\n";
						}
					}
					else if (strcmp(command, "+Fv") == 0) {
						if (flipImage(inputFilename, outputFilename, FLIP_VERTICALLY)) {
							cout << "Flip image vertically successfully\n";
						}
						else {
							cout << "Failed to flip image vertically\n";
						}
					}
					else if (strcmp(command, "+Fba") == 0) {
						if (flipImage(inputFilename, outputFilename, FLIP_BOTH_AXES)) {
							cout << "Flip image both axes successfully\n";
						}
						else {
							cout << "Failed to flip image both axes\n";
						}
					}
					else if (strcmp(command, "+Sxf") == 0 && i + 1 < argc) {
						strcpy(command, argv[++i]);
						float factor = atof(command);
						if (scaleImageByFactor(inputFilename, outputFilename, SCALE_X, factor)) {
							cout << "Scale image's x axis by factor successfully\n";
						}
						else {
							cout << "Failed to scale image's x axis by factor\n";
						}
					}
					else if (strcmp(command, "+Syf") == 0 && i + 1 < argc) {
						strcpy(command, argv[++i]);
						float factor = atof(command);
						if (scaleImageByFactor(inputFilename, outputFilename, SCALE_Y, factor)) {
							cout << "Scale image's y axis by factor successfully\n";
						}
						else {
							cout << "Failed to scale image's y axis by factor\n";
						}
					}
					else if (strcmp(command, "+Cxf") == 0 && i + 5 < argc) {
						strcpy(command, argv[++i]);
						int left = atoi(command);
						strcpy(command, argv[++i]);
						int top = atoi(command);
						strcpy(command, argv[++i]);
						int width = atoi(command);
						strcpy(command, argv[++i]);
						int height = atoi(command);
						strcpy(command, argv[++i]);
						float factor = atof(command);
						if (clipAndScaleImageByFactor(inputFilename, outputFilename, left, top, width, height, SCALE_X, factor)) {
							cout << "Clip and scale image's x axis by factor successfully\n";
						}
						else {
							cout << "Failed to clip and scale image's x axis by factor\n";
						}
					}
					else if (strcmp(command, "+Cyf") == 0 && i + 5 < argc) {
						strcpy(command, argv[++i]);
						int left = atoi(command);
						strcpy(command, argv[++i]);
						int top = atoi(command);
						strcpy(command, argv[++i]);
						int width = atoi(command);
						strcpy(command, argv[++i]);
						int height = atoi(command);
						strcpy(command, argv[++i]);
						float factor = atof(command);
						if (clipAndScaleImageByFactor(inputFilename, outputFilename, left, top, width, height, SCALE_Y, factor)) {
							cout << "Clip and scale image's y axis by factor successfully\n";
						}
						else {
							cout << "Failed to clip and scale image's y axis by factor\n";
						}
					}
					else if (strcmp(command, "+Cc") == 0) {
						if (changeContrastOfImage(inputFilename, outputFilename)) {
							cout << "Change contrast of image successfully\n";
						}
						else {
							cout << "Failed to change contrast of image\n";
						}
					}
					else if (strcmp(command, "+Wfl") == 0) {
						voiLutFunc = LINEAR;
						cout << "Set VOI LUT function to LINEAR successfully\n";
					}
					else if (strcmp(command, "+Wfs") == 0) {
						voiLutFunc = SIGMOID;
						cout << "Set VOI LUT function to SIGMOID successfully\n";
					}
					else if (strcmp(command, "+Wi") == 0 && i + 1 < argc) {
						strcpy(command, argv[++i]);
						signed int numOfWindows = atoi(command);
						if (applyExistingVOIWindows(inputFilename, outputFilename, numOfWindows, voiLutFunc)) {
							cout << "Apply existing VOI windows successfully\n";
						}
						else {
							cout << "Failed to apply existing VOI windows\n";
						}
					}
					else if (strcmp(command, "+Wm") == 0) {
						if (applyMinMaxVOIWindows(inputFilename, outputFilename, voiLutFunc)) {
							cout << "Apply min max VOI windows successfully\n";
						}
						else {
							cout << "Failed to apply min max VOI windows\n";
						}
					}
					else if (strcmp(command, "+Wh") == 0 && i + 1 < argc) {
						strcpy(command, argv[++i]);
						signed int ignoringPercent = atoi(command);
						if (applyHistogramVOIWindows(inputFilename, outputFilename, ignoringPercent, voiLutFunc)) {
							cout << "Apply histogram VOI windows successfully\n";
						}
						else {
							cout << "Failed to apply histogram VOI windows\n";
						}
					}
					else if (strcmp(command, "+Ww") == 0 && i + 2 < argc) {
						strcpy(command, argv[++i]);
						float center = atof(command);
						strcpy(command, argv[++i]);
						float width = atof(command);
						if (applySettingVOIWindows(inputFilename, outputFilename, center, width, voiLutFunc)) {
							cout << "Apply setting VOI windows successfully\n";
						}
						else {
							cout << "Failed to apply setting VOI windows\n";
						}
					}
					else if (strcmp(command, "+Cp") == 0) {
						if (createGSPS(inputFilename, outputFilename)) {
							cout << "Create GSPS successfully\n";
						}
						else {
							cout << "Failed to create GSPS\n";
						}
					}
					else if (strcmp(command, "+Alp") == 0 && i + 4 < argc) {
						strcpy(command, argv[++i]);
						float pointX1 = atof(command);
						strcpy(command, argv[++i]);
						float pointY1 = atof(command);
						strcpy(command, argv[++i]);
						float pointX2 = atof(command);
						strcpy(command, argv[++i]);
						float pointY2 = atof(command);
						float length = addLineToGSPS(inputFilename, outputFilename, "MEASUREMENT", pointX1, pointY1, pointX2, pointY2);
						if (length != -1) {
							cout << "Add line to GSPS successfully\n";
							cout << "Length of line: " << length << "mm\n";
						}
						else {
							cout << "Failed to add line to GSPS\n";
						}
					} if (strcmp(command, "+Atp") == 0 && i + 3 < argc) {
						strcpy(command, argv[++i]);
						float pointX = atof(command);
						strcpy(command, argv[++i]);
						float pointY = atof(command);
						strcpy(command, argv[++i]);
						bool flag = addTextToGSPS(inputFilename, outputFilename, "MEASUREMENT", pointX, pointY, command);
						if (flag) {
							cout << "Add text to GSPS successfully\n";
						}
						else {
							cout << "Failed to add text to GSPS\n";
						}
					}
					else if (strcmp(command, "+Ma") == 0 && i + 8 < argc) {
						strcpy(command, argv[++i]);
						float pointX1 = atof(command);
						strcpy(command, argv[++i]);
						float pointY1 = atof(command);
						strcpy(command, argv[++i]);
						float pointX2 = atof(command);
						strcpy(command, argv[++i]);
						float pointY2 = atof(command);
						strcpy(command, argv[++i]);
						float pointX3 = atof(command);
						strcpy(command, argv[++i]);
						float pointY3 = atof(command);
						strcpy(command, argv[++i]);
						float pointX4 = atof(command);
						strcpy(command, argv[++i]);
						float pointY4 = atof(command);
						float angle = measureAngle(pointX1, pointY1, pointX2, pointY2, pointX3, pointY3, pointX4, pointY4);
						if (angle != -1) {
							cout << "Measure angle successfully\n";
							cout << "Angle: " << angle << " degrees\n";
						}
						else {
							cout << "Failed to measure angle\n";
						}
					}
					else if (strcmp(command, "+Cbmp") == 0) {
						if (convertDcmToBMP(inputFilename, outputFilename)) {
							cout << "Convert dcm file to bmp file successfully\n";
						}
						else {
							cout << "Failed to convert dcm file to bmp file";
						}
					}
					else if (strcmp(command, "+Cjpg") == 0) {
						if (convertDcmToJPG(inputFilename, outputFilename)) {
							cout << "Convert dcm file to jpg file successfully\n";
						}
						else {
							cout << "Failed to convert dcm file to jpg file";
						}
					}
					else if (strcmp(command, "+Cpng") == 0) {
						if (convertDcmToPNG(inputFilename, outputFilename)) {
							cout << "Convert dcm file to png file successfully\n";
						}
						else {
							cout << "Failed to convert dcm file to png file";
						}
					}
					else if (strcmp(command, "+Rtp") == 0) {
						if (removeLastTextFromGSPS(inputFilename, outputFilename, "MEASUREMENT")) {
							cout << "Remove last text from GSPS successfully\n";
						}
						else {
							cout << "Failed to remove last text from GSPS";
						}
					}
					else if (strcmp(command, "+Rlp") == 0) {
						if (removeLastLineFromGSPS(inputFilename, outputFilename, "MEASUREMENT")) {
							cout << "Remove last line from GSPS successfully\n";
						}
						else {
							cout << "Failed to remove last line from GSPS";
						}
					}
				}
			}
			else {
				cout << "Error: wrong command format. Enter '-h' to view help\n";
				cout << "Exit.\n";
				exit(2);
			}
		}
		else {
			cout << "Error: wrong command format. Enter '-h' to view help\n";
			cout << "Exit.\n";
			exit(2);
		}
	}
}

int main(int argc, char const* argv[]) {
	load(argc, argv);
	const char* dcmFilenames[] = {
		"00000001",
		"00000002",
		"00000003",
		"00000004",
		"00000005",
		"00000006",
		"00000007",
		"00000008",
		"00000009",
		"00000010"
	};

	createDcmDir(10, dcmFilenames);

	SDL_Init(SDL_INIT_EVERYTHING);

	if (TTF_Init() < 0) return 0;

	SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_Surface* surface = IMG_Load("apply_1st_existing_VOI_windows.0.bmp");

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_DestroyTexture(texture);


	TTF_Font* font = NULL;
	font = TTF_OpenFont("arial.ttf", 24);

	DcmFileFormat dfile;
	DcmFileFormat gspsfile;

	if (dfile.loadFile("00000001").bad()) return 0;
	if (gspsfile.loadFile("gsps.dcm").bad()) return 0;

	DVPresentationState pstate;

	if (pstate.read(*gspsfile.getDataset()).bad()) return 0;
	if (pstate.attachImage(&dfile, OFFalse).bad()) return 0;

	for (size_t layer = 0; layer < pstate.getNumberOfGraphicLayers(); ++layer) {
		cout << "Layer's name: " << pstate.getGraphicLayerName(layer) << "\n";

		size_t max = pstate.getNumberOfGraphicObjects(layer);

		cout << "Number of graphic objects: " << max << "\n";

		DVPSGraphicObject* pgraphic = NULL;

		for (size_t graphicidx = 0; graphicidx < max; ++graphicidx) {
			pgraphic = pstate.getGraphicObject(layer, graphicidx);

			if (pgraphic) {
				size_t j = pgraphic->getNumberOfPoints();
				for (size_t i = 0; i <= j / 2; ++i) {
					Float32 fx = 0.0, fy = 0.0;
					if (EC_Normal == pgraphic->getPoint(i, fx, fy)) {
						cout << fx << "/" << fy << "\n";
					}
					Float32 nfx = 0.0, nfy = 0.0;
					if (EC_Normal == pgraphic->getPoint(++i, nfx, nfy)) {
						cout << nfx << "/" << nfy << "\n";
					}
					SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0XFF);
					SDL_RenderDrawLineF(renderer, fx, fy, nfx, nfy);
				}
			}
		}

		max = pstate.getNumberOfTextObjects(layer);

		cout << "Number of text objects: " << max << "\n";

		DVPSTextObject* ptext = NULL;

		for (size_t textidx = 0; textidx < max; ++textidx) {
			ptext = pstate.getTextObject(layer, textidx);

			if (ptext) {
				Float32 fx = 0.0, fy = 0.0;
				if (ptext->haveAnchorPoint()) {
					fx = ptext->getAnchorPoint_x();
					fy = ptext->getAnchorPoint_y();

					cout << fx << "/" << fy << "\n";
				}

				cout << ptext->getText() << "\n";

				SDL_Color color = { 255, 255, 255 };
				surface = TTF_RenderText_Solid(font, ptext->getText(), color);
				texture = SDL_CreateTextureFromSurface(renderer, surface);

				int text_width = surface->w;
				int text_height = surface->h;

				SDL_Rect rect;

				rect.x = fx;
				rect.y = fy;
				rect.w = text_width;
				rect.h = text_height;

				SDL_RenderCopy(renderer, texture, NULL, &rect);

				SDL_DestroyTexture(texture);
				SDL_FreeSurface(surface);

			}
		}
	}

	pstate.detachImage();

	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		SDL_RenderPresent(renderer);
	}


	TTF_CloseFont(font);

	TTF_Quit();

	SDL_Quit();
}