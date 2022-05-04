/*
	Linux Module Memory library

*/

//====================================================================
//include

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//global var

//====================================================================
//functions

//--------------------------------------------------------------------
void*		GrLxmPageAlloc( __u32 A_Size )
{
	// local -------------------
		unsigned __s32	Tv_PgCnt;
	// code --------------------
		Tv_PgCnt		=	get_order( A_Size );
		return	(void*)__get_free_pages( GFP_ATOMIC, Tv_PgCnt );
}
//--------------------------------------------------------------------
void	GrLxmPageFree( void* A_Ptr, __u32 A_Size )
{
	// local -------------------
		unsigned __s32	Tv_PgCnt;
	// code --------------------
		if ( 0 != A_Size )
		{
			Tv_PgCnt		=	get_order( A_Size );
			free_pages( (unsigned long)A_Ptr, Tv_PgCnt );
		}
}
//--------------------------------------------------------------------
__u32	GrLxmCalcPageSize( __u32 A_Size )
{
		return	(A_Size + (PAGE_SIZE - 1)) & PAGE_MASK;
}
//--------------------------------------------------------------------
