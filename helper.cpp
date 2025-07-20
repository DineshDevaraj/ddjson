
#include <cstdarg>

#include "helper.h"
#include "writer.h"
#include "doc.h"

namespace ddjson
{
   void Helper_t::free_node(Node_t * &pnode)
   {
      if(NULL == pnode)
         return;

      if(Valtype::Array == pnode->vtype or
            Valtype::Object == pnode->vtype)
      {
         Node_t *cur = NULL;
         Node_t *next = NULL;
         for(cur = pnode->vobj; cur; cur = next)
         {
            next = cur->pnext;
            free_node(cur);
         }
      }
                                
      delete pnode;
      pnode = NULL;

      return;
   }

   template <> int Helper_t::print(FILE * &fh, const char *fmt, ...)
   {
      va_list args;
      va_start(args, fmt);
      return vfprintf(fh, fmt, args);
   }
   
   template <> int Helper_t::print(char * &cp, const char *fmt, ...)
   {
      va_list args;
      va_start(args, fmt);
      int len = vsprintf(cp, fmt, args);
      cp += len;
      return len;
   }

   template <> int Helper_t::print(ostream * &os, const char *fmt, ...)
   {
      va_list args;
      va_start(args, fmt);
      int len = vsnprintf(0, 0, fmt, args);
      if(len > 0)
      {
         string str;
         str.resize(len);
         vsprintf(&str[0], fmt, args);
         (*os) << str;
      }
      return len;
   }

   template <typename tn> /* pn - pointer to node */
   int Helper_t::write(tn * &ptr, Node_t *pn, const char *pad, int lev)
   {
      string fmt;
      int len = 0;
      Node_t *itr = NULL;
      Writer_t &wrt = pn->pdoc->writer;

      if(pad) for(int I = 0; I < lev; I++)
         len += print(ptr, "%s", pad);

      if(not pn->name.empty())
      {
         len += print(ptr, "\"%s\"", pn->name.data());
         if(pad) print(ptr, " : ");
         else print(ptr, ":");
      }

      switch(pn->vtype)
      {
         case Valtype::Int : len += print(ptr, wrt.int_format.data(), pn->vint); 
                             break;

        case Valtype::Bool : len += print(ptr, "%s", pn->vbool ? "true" : "false");
                              break;

         case Valtype::Float : len += print(ptr, wrt.float_format.data(), pn->vreal); 
                               break;

         case Valtype::String :fmt  = '"'; 
                               fmt += wrt.str_format.data();
                               fmt += '"';
                               len += print(ptr, fmt.data(), pn->vstr.data()); 
                                break;

         case Valtype::Array : len += print(ptr, "[");
                               if(pn->vobj)
                               {
                                  if(pad) print(ptr, "\n");
                                  for(itr = pn->vobj; itr; )
                                  {
                                     len += write(ptr, itr, pad, lev + 1);
                                     itr  = itr->pnext;
                                     if(itr) len += print(ptr, ",");
                                     if(pad) len += print(ptr, "\n");
                                  }
                                  if(pad) for(int I = 0; I < lev; I++)
                                     len += print(ptr, "%s", pad);
                               }
                               len += print(ptr, "]");
                               break;

         case Valtype::Object : len += print(ptr, "{");
                                if(pn->vobj)
                                {
                                   if(pad) print(ptr, "\n");
                                   for(itr = pn->vobj; itr; )
                                   {
                                      len += write(ptr, itr, pad, lev + 1);
                                      itr = itr->pnext;
                                      if(itr) len += print(ptr, ",");
                                      if(pad) len += print(ptr, "\n");
                                   }
                                   if(pad) for(int I = 0; I < lev; I++)
                                      len += print(ptr, "%s", pad);
                                }
                                len += print(ptr, "}");
                                break;

         case Valtype::Null : len += print(ptr, "null"); break;
      }

      return len;
   }
}