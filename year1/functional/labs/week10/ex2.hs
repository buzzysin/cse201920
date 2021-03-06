-- * Ex 2.1
-- ! Starred question

data Maybe' a = Just a | Nothing

instance Monad Maybe' where
  return :: a -> Maybe' a
  return x = Just x

  (>>=) :: Maybe a -> (a -> Maybe b) -> Maybe b
  (>>=) Nothing _   = Nothing
  (>>=) (Just x) f  = f x

  (>>) :: Maybe a -> Maybe b -> Maybe b
  (>>) Nothing _    = Nothing
  (>>) (Just _) mb  = mb 
