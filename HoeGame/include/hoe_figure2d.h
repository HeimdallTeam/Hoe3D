
/**
   @file    hoe_figure2d.h
   @date    Nov 2004
   @version $Revision: 1.5 $
   @brief   Rozlo�en� 2d sc�ny.
*/

#ifndef _HOE_FIGURE_2D_H_
#define _HOE_FIGURE_2D_H_

#include <hoe_structures.h>
#include <hoe3d.h>
#include "hoe_game.h"
#include "hoe_fs.h"
#include "hoe_utils.h"
#include "hoe_gui.h"

BEGIN_HOEGAME

class FigureFile : public TextFile
{
};

/**
 * @brief	Rozlo�en� 2d sc�ny.
 * No jasne.. to bude uprava tohoto, tohle bude obsahovat informace
 * o tom co a jak a v jakem poradi se ma kreslit.
 * pres nastavitelne parametry se budou moci pozmenovat infa
 * proste takovy 2d engine, s pojmenovanymi oblastmi
 */
class Hoe2DFigureBase : public Hoe2DControl
{
public:
	/** Kontruktor */
	Hoe2DFigureBase();
	/** Destruktor */
	virtual ~Hoe2DFigureBase();
	/** Funkce ktera nacte ze souboru dalsi prvky */
	bool Load(const char * fname);
	virtual Gui::Base * CreateGUI(const char * type) = 0;
};

class Hoe2DFigure : public Hoe2DFigureBase
{
protected:
	// seznam vykresleni
	HoeCore::List<Gui::Item*> m_list;
public:
	virtual ~Hoe2DFigure() { Clear(); }
	void Clear();
	virtual Gui::Base * CreateGUI(const char * type);
	virtual void Draw(IHoe2D * hoe2d);
	Gui::Item * GetItem(const char * name, Gui::EType type);
	Gui::Item * ReqItem(const char * name, Gui::EType type);
	void Move(const float x, const float y, bool & act);
	bool Click(const float x, const float y);
	const HoeCore::List<Gui::Item*> & GetItems() const { return m_list; }
};

END_HOEGAME

#endif // _HOE_FIGURE_2D_H_

