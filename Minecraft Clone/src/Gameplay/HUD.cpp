#include "HUD.h"




void HUD::render() {
	m_renderer.begin();
	
	drawCrosshair();
	drawHotbar();
	drawXPBar();
	drawHearts();
	//drawFood();

	m_renderer.flush(&m_window);
}


void HUD::drawCrosshair() {
	m_renderer.drawQuad(
		(float)m_window.getWidth() / 2 - (HUDSprites::CROSSHAIR_SIZE_5 / 2.0f), 
		(float)m_window.getHeight() / 2 - (HUDSprites::CROSSHAIR_SIZE_5 / 2.0f), 
		HUDSprites::CROSSHAIR_SIZE_5, HUDSprites::CROSSHAIR_SIZE_5,
		HUDSprites::CROSSHAIR_U0 / HUDSprites::WIDTH, HUDSprites::CROSSHAIR_V0 / HUDSprites::HEIGHT,
		HUDSprites::CROSSHAIR_U1 / HUDSprites::WIDTH, HUDSprites::CROSSHAIR_V1 / HUDSprites::HEIGHT);
}

void HUD::drawHotbar() {
	m_renderer.drawQuad(
		(float)m_window.getWidth() / 2.0f - (HUDSprites::HOTBAR_WIDTH * 5.0f / 2.0f),
		(float)m_window.getHeight() * 0.9f,
		HUDSprites::HOTBAR_WIDTH * 5.0f, HUDSprites::HOTBAR_HEIGHT * 5.0f,
		HUDSprites::HOTBAR_U0 / HUDSprites::WIDTH, HUDSprites::HOTBAR_V1 / HUDSprites::HEIGHT,
		HUDSprites::HOTBAR_U1 / HUDSprites::WIDTH, HUDSprites::HOTBAR_V0 / HUDSprites::HEIGHT);
}

void HUD::drawXPBar() {
	m_renderer.drawQuad(
		(float)m_window.getWidth() / 2.0f - (HUDSprites::XPBAR_WIDTH * 5.0f / 2.0f),
		(float)m_window.getHeight() * 0.88f - 2,
		HUDSprites::XPBAR_WIDTH * 5.0f, HUDSprites::XPBAR_HEIGHT * 3.5f,
		HUDSprites::XPBAR_U0 / HUDSprites::WIDTH, HUDSprites::XPBAR_V1 / HUDSprites::HEIGHT,
		HUDSprites::XPBAR_U1 / HUDSprites::WIDTH, HUDSprites::XPBAR_V0 / HUDSprites::HEIGHT);
}

void HUD::drawHearts() {

	for (int i = 0; i < 10; i++) {
		m_renderer.drawQuad(
			(float)m_window.getWidth() / 2.0f - (HUDSprites::HEART_WIDTH * 5.0f / 2.0f) - HUDSprites::HOTBAR_WIDTH * 5.0f / 2.0f + i * HUDSprites::HEART_WIDTH * 2.69f + 16,
			(float)m_window.getHeight() * 0.85f,
			HUDSprites::HEART_WIDTH * 3.0f, HUDSprites::HEART_HEIGHT * 3.0f,
			HUDSprites::HEART_HOLDER_U0 / HUDSprites::WIDTH, HUDSprites::HEART_HOLDER_V1 / HUDSprites::HEIGHT,
			HUDSprites::HEART_HOLDER_U1 / HUDSprites::WIDTH, HUDSprites::HEART_HOLDER_V0 / HUDSprites::HEIGHT);
	}
	for (int i = 0; i < 10; i++) {
		
		m_renderer.drawQuad(
			(float)m_window.getWidth() / 2.0f - (HUDSprites::HEART_WIDTH * 5.0f / 2.0f) - HUDSprites::HOTBAR_WIDTH * 5.0f / 2.0f + i * HUDSprites::HEART_WIDTH * 2.69f + 16,
			(float)m_window.getHeight() * 0.85f,
			HUDSprites::HEART_WIDTH * 3.0f, HUDSprites::HEART_HEIGHT * 3.0f,
			HUDSprites::HEART_U0 / HUDSprites::WIDTH, HUDSprites::HEART_V1 / HUDSprites::HEIGHT,
			HUDSprites::HEART_U1 / HUDSprites::WIDTH, HUDSprites::HEART_V0 / HUDSprites::HEIGHT);
	}
}

void HUD::drawFood() {
	for (int i = 0; i < 10; i++) {
		m_renderer.drawQuad(
			(float)m_window.getWidth() / 2.0f - (HUDSprites::FOOD_HEIGHT * 5.0f / 2.0f) + HUDSprites::HOTBAR_WIDTH * 5.0f / 2.0f - i * HUDSprites::FOOD_HEIGHT * 2.69f - 5,
			(float)m_window.getHeight() * 0.85f,
			HUDSprites::FOOD_WIDTH * 3.0f, HUDSprites::FOOD_HEIGHT * 3.0f,
			HUDSprites::FOOD_HOLDER_U0 / HUDSprites::WIDTH, HUDSprites::FOOD_HOLDER_V1 / HUDSprites::HEIGHT,
			HUDSprites::FOOD_HOLDER_U1 / HUDSprites::WIDTH, HUDSprites::FOOD_HOLDER_V0 / HUDSprites::HEIGHT);
	}

	for (int i = 0; i < 10; i++) {
		m_renderer.drawQuad(
			(float)m_window.getWidth() / 2.0f - (HUDSprites::FOOD_HEIGHT * 5.0f / 2.0f) + HUDSprites::HOTBAR_WIDTH * 5.0f / 2.0f - i * HUDSprites::FOOD_HEIGHT * 2.69f - 5,
			(float)m_window.getHeight() * 0.85f,
			HUDSprites::FOOD_WIDTH * 3.0f, HUDSprites::FOOD_HEIGHT * 3.0f,
			HUDSprites::FOOD_U0 / HUDSprites::WIDTH, HUDSprites::FOOD_V1 / HUDSprites::HEIGHT,
			HUDSprites::FOOD_U1 / HUDSprites::WIDTH, HUDSprites::FOOD_V0 / HUDSprites::HEIGHT);
	}
}
