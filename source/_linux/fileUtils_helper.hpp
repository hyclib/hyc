//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// business logic layer network proxy
// author:       lipengxiang
// description:  FileUtils的linux实现
//

#ifndef __BP_FILEUTILS_HELPER_HPP_20110921145907__
#define __BP_FILEUTILS_HELPER_HPP_20110921145907__

namespace hyc
{
	class _Wildcard : public MemoryAllocObject
	{
		enum State
		{
			State_start,
			State_one,
			State_char,
		};
	public:
		_Wildcard(const tchar* str)
		{
			mStr = str;
			mLastStartIndex = -1;
			mStrLen = mStr.size();
		}

		//-----------------------------------
		bool	Check(const tchar* str)
		{
			mIndex = 0;
			integer nIndex = 0;
			integer nState = State_char;
			mLastCharIndex = -1;
			tchar c;
			while (true)
			{
				if (mIndex >= mStrLen && nState != State_start)
				{
					return false;
				}
				else if (mIndex >= mStrLen)
				{
					mIndex = mStrLen - 1;
				}

				c = mStr[mIndex];
				switch(c)
				{
				case _T('*'):
					{
						mLastStartIndex = nIndex;
						mIndex++;

						// 如果通配符*是最后一个字符
						// 匹配一定成功
						if (mIndex == mStrLen)
						{
							return true;
						}

						nState = State_start;
						break;
					}
				case _T('?'):
					{
						if (nState == State_start)
						{
							// *? == * 这种形式基本等价，
							// 仅后面的一个占位符处理差异
							mLastStartIndex = mIndex;

							mIndex++;
							// 因为?要占用一个位，所以nIndex自增
							int r = _Inc(str, nIndex);
							if(-1 != r)
							{
								return r == 1;
							}
						}
						else
						{
							mIndex++;
							int r = _Inc(str, nIndex);
							if(-1 != r)
							{
								return r == 1;
							}
							nState = State_char;
						}

						break;
					}
				default:
					{
						if (mStr[mIndex] == str[nIndex])
						{
							// 标记第一个匹配成功的字符串位置
							// 用于回溯
							if (mLastCharIndex < 0 && nState == State_start)
							{
								mLastCharIndex = nIndex;
							}

							mIndex++;
							int r = _Inc(str, nIndex);
							if(-1 != r)
							{
								return r == 1;
							}

						}
						else
						{

							bool last = true;
							if (nState == State_char)
							{
								return false;
							}
							else if(nState == State_one)
							{
								nState = State_char;
							}
							else
							{
								last = false;
								mIndex = mLastStartIndex + 1;

								// 回溯
								if (mLastCharIndex >= 0)
								{
									nIndex = mLastCharIndex;
									mLastCharIndex = -1;
								}
							}

							int r = _Inc(str, nIndex, last);
							if(-1 != r)
							{
								return r == 1;
							}
						}
						break;
					}
				}

			}
		}

		int		_Inc(const tchar* str, integer& nIndex, bool last = true)
		{
			nIndex++;
			if (_T('\0') == str[nIndex])
			{
				if (mIndex == mStrLen - 1)
				{
					return last;
				}

				// 判断后面的字符是否全是*
				// 如果是，匹配成功，否则失败
				integer i = mIndex;
				while(i < mStrLen)
				{
					if (mStr[i] != _T('*'))
					{
						return 0;
					}
					i++;
				}
				return 1;
			}
			return -1;
		}

		String			mStr;
		integer         mLastStartIndex;
		integer         mLastCharIndex;
		integer         mStrLen;
		integer         mIndex;
	};

} // end namespace hyc

#endif // end of define __BP_FILEUTILS_HELPER_HPP_20110921145907__
