
#include "StdAfx.h"
#include "../include/hoe_path.h"

BEGIN_HOEGAME
namespace AI {
namespace Path {
/*
potrebuje:
 ohodnocenou frontu
 flag closed
 heuristiku
 pocitaci funkci
 vracet sousedy
 funkce w (pomerna vzdalenost)

 g = vzdalenost od startu
 h = heuristika
 f = pomerovaci funkce g + h

AStar_algoritmus
   s.g = 0           // s je startovn� pol��ko
   s.h = Heuristika(s)
   s.f = s.g + s.h
   s.p�edchoz� = null
   Open.push s
   Dokud je Open nepr�zdn�
         Open.pop n // n m� nejmen�� f
         pokud n je c�lov� pol��ko
               konstruuj cestu
               vra� �sp�ch
         pro ka�d�ho souseda n' vrcholu n
               nov�g = n.g + w(n,n')
               pokud n' je v Open nebo Closed, a n'.g <= nov�g
                     zpracuj dal��ho souseda
               n'.p�edchoz� = n
               n'.g = nov�g
               n'.h = Heuristika(n')
               n'.f = n'.g + n'.h
               pokud je n' v Closed
                     vypus� ho z Closed
               pokud n' je�t� nen� v Open
                     Open.push n'
         Closed.push n
   vra� ne�sp�ch // cesta neexistuje
*/

} // namespace Path
} // namespace AI

END_HOEGAME


