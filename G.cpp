#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#define MAXN 100010
using namespace std;

struct segmentnode
{
	int l, r;
} segment[MAXN];

struct node
{
	int num, next;
} e[MAXN];

int trie[MAXN][26], g[MAXN], num[MAXN], sum[MAXN], maxson[MAXN], d[MAXN], head[MAXN], tail[MAXN], fa[MAXN], a[MAXN], cut[MAXN];
long long c[2][MAXN], ans;
int n, N, m, tot;

bool operator < (segmentnode a, segmentnode b)
{
	return a.l < b.l;
}

int lowbit (int x)
{
	return x & -x;
}

void add (int now, int x, int y)
{
	while (x <= n)
	{
		c[now][x] += y;
		x += lowbit (x);
	}
}

long long calc (int now, int x)
{
	long long ans = 0;
	while (x)
	{
		ans += c[now][x];
		x -= lowbit(x);
	}
	return ans;
}

void paint (int x, int y)
{
	add (0, x, 1);
	add (1, x, x);
	add (0, y + 1, -1);
	add (1, y + 1, -(y + 1));
}

long long getsum (int x, int y)
{
	long long ans = calc (0, y) * (long long) (y + 1) + calc (1, x - 1);
	ans -= calc (1, y) + calc (0, x - 1) * (long long) x;
	return ans;
}

void addedge (int a, int b)
{
	e[++ tot].num = b;
	e[tot].next = g[a];
	g[a] = tot;
}

void Build_Trie ()
{
	int i, tmp, now, head, tail;
	head = tail = 1, d[1] = 1, fa[1] = 0;
	for (i = 0; i < 26; i ++)
		trie[0][i] = 1;
	while (head <= tail)
	{
		now = d[head];
		for (i = 0; i < 26; i ++)
			if (trie[now][i])
			{
				tmp = fa[now];
				while (!trie[tmp][i]) tmp = fa[tmp];
				fa[trie[now][i]] = trie[tmp][i];
				addedge (trie[tmp][i], trie[now][i]);
				d[++ tail] = trie[now][i];
			}
		head ++;
	}
}

void search1 (int now)
{
	int i;
	sum[now] = 1; maxson[now] = 0;
	for (i = g[now]; i; i = e[i].next)
	{
		search1 (e[i].num);
		sum[now] += sum[e[i].num];
		if (sum[maxson[now]] < sum[e[i].num]) maxson[now] = e[i].num;
	}
}

void search2 (int now, int a)
{
	int i;
	num[now] = ++ tot; head[now] = a;
	if (maxson[now]) search2 (maxson[now], a);
	for (i = g[now]; i; i = e[i].next)
	{
		if (e[i].num == maxson[now]) continue;
		search2 (e[i].num, e[i].num);
	}
	tail[now] = tot;
}

void work (int now)
{
	if (!now || cut[head[now]] >= num[now]) return;
	if (cut[head[now]])
	{
		ans += getsum (cut[head[now]] + 1, num[now]);
	}else
	{
		ans += getsum (num[head[now]], num[now]);
		work (fa[head[now]]);
	}
	cut[head[now]] = num[now];
}

int main ()
{
	freopen ("g.in", "r", stdin);
	freopen ("g.out", "w", stdout);
	int i, x, test, maxr;
	char s[2];
	scanf ("%d", &test);
	while (test --)
	{
		memset (g, 0, sizeof g); tot = 0;
		memset (trie, 0, sizeof trie);
		memset (c, 0, sizeof c);
		scanf ("%d", &n);
		for (i = 1; i <= n - 1; i ++)
		{
			scanf ("%d%s", &x, s);
			trie[x][s[0] - 'a'] = i + 1;
		}
		Build_Trie ();
		search1 (1);
		tot = 0;
		search2 (1, 1);
		scanf ("%d", &m);
		while (m --)
		{
			scanf ("%d", &x);
			if (x == 1)
			{
				scanf ("%d", &N);
				for (i = 1; i <= N; i ++)
				{
					scanf ("%d", &a[i]);
					segment[i].l = num[a[i]], segment[i].r = tail[a[i]];
				}
				sort (segment + 1, segment + 1 + N);
				maxr = 0;
				for (i = 1; i <= N; i ++)
				{
					if (maxr >= segment[i].r) continue;
					paint (segment[i].l, segment[i].r);
					maxr = segment[i].r;
				}
			} else
			{
				scanf ("%d", &N); ans = 0;
				for (i = 1; i <= N; i ++)
				{
					scanf ("%d", &a[i]);
					work (a[i]);
				}
				printf("%I64d\n", ans);
				for (i = 1; i <= N; i ++)
				{
					while (a[i])
					{
						cut[head[a[i]]] = 0;
						a[i] = fa[head[a[i]]];
					}
				}
			}
		}
	}
}
