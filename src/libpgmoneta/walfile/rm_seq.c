/*
 * Copyright (C) 2025 The pgmoneta community
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may
 * be used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 #include <utils.h>
 #include <wal.h>
 #include <walfile/rm.h>
 #include <walfile/rm_seq.h>

char*
pgmoneta_wal_seq_desc(char* buf, struct decoded_xlog_record* record)
{
   char* rec = XLOG_REC_GET_DATA(record);
   uint8_t info = XLOG_REC_GET_INFO(record) & ~XLR_INFO_MASK;
   struct xl_seq_rec* xlrec = (struct xl_seq_rec*) rec;
   char* dbname = NULL;
   char* relname = NULL;
   char* spcname = NULL;

   if (info == XLOG_SEQ_LOG)
   {

      if (pgmoneta_get_database_name(xlrec->node.dbNode, &dbname))
      {
         goto error;
      }

      if (pgmoneta_get_relation_name(xlrec->node.relNode, &relname))
      {
         goto error;
      }

      if (pgmoneta_get_tablespace_name(xlrec->node.spcNode, &spcname))
      {
         goto error;
      }

      buf = pgmoneta_format_and_append(buf, "rel %s/%s/%s",
                                       spcname, dbname,
                                       relname);

   }

   free(dbname);
   free(spcname);
   free(relname);
   return buf;

error:
   free(dbname);
   free(spcname);
   free(relname);

   return NULL;
}
