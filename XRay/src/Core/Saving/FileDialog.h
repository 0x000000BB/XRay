#pragma once
#include <string>
#include <commdlg.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace XRay {

	class FileDialog{

	public:
		FileDialog(GLFWwindow* win) : window(win) {}

		std::string OpenFile(const char* filter) {
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window(window);
			ofn.lpstrFile = szFile; 
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			if (GetOpenFileName(&ofn) == TRUE) {
				return ofn.lpstrFile;
			}
			return std::string();
		}

		std::string SaveFile(const char* filter) {
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window(window);
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			if (GetSaveFileName(&ofn) == TRUE) {
				return ofn.lpstrFile;
			}
			return std::string();
		}

	private:
		GLFWwindow* window;

	};



}